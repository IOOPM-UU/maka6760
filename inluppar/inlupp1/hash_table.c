#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"

#define Init_Capacity 17
#define Default_Load_Factor 0.75f

struct entry
{
    elem_t key;
    elem_t value;
    entry_t *next; // points to the next entry (possibly NULL)
};

// Every bucket is a pointer to the first entry in its list, or NULL if the
// bucket is empty. There are no dummy entries, so an empty bucket only costs
// one pointer.
struct hash_table
{
    size_t size;
    size_t capacity; //current number of buckets 
    float load_factor;   // grow when size / capacity exceeds this
    entry_t **buckets; // malloc:ed array of capacity pointers
    ioopm_hash_function *hash_fn;
    ioopm_eq_function *key_eq_fn;
};

struct hash_table_iterator
{
    ioopm_hash_table_t *ht;
    size_t current_bucket;
    entry_t *current_entry;
};

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
    entry_t *new_entry = malloc(sizeof(entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = next;
    return new_entry;
}

// Only the entry is freed, keys and values are owned by the user.
static entry_t *entry_destroy(entry_t *entry)
{
    free(entry);
    return NULL;
}

// Moves the iterator to the first non-empty bucket starting at current_bucket,
// or to the end if there is none.
static void find_next_non_empty_bucket(ioopm_hash_table_iterator_t *it)
{
    while (it->current_bucket < it->ht->capacity &&
           it->ht->buckets[it->current_bucket] == NULL)
    {
        it->current_bucket++;
    }

    if (it->current_bucket < it->ht->capacity)
    {
        it->current_entry = it->ht->buckets[it->current_bucket];
    }
    else
    {
        it->current_entry = NULL;
    }
}

// Returns a pointer to the link that points to the entry with the key. The link
// is either the bucket itself or the next field of the entry before it.
// If the key does not exist, the link points to NULL (the end of the list),
// which is exactly where a new entry should be placed.
static entry_t **find_entry_link(ioopm_hash_table_t *ht, const elem_t key)
{
    size_t bucket = ht->hash_fn(key) % ht->capacity;
    entry_t **link = &ht->buckets[bucket];
    while (*link != NULL && !ht->key_eq_fn((*link)->key, key))
    {
        link = &(*link)->next;
    }
    return link;
}

static size_t next_capacity(size_t current)
{   
    static const size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
    size_t no_primes = sizeof(primes) / sizeof(primes[0]);

    for (size_t i = 0; i < no_primes; i++)
    {
        if (primes[i] > current)
        {
            return primes[i];
        }
    }
    return current * 2 +1;
}

// Grows the bucket array of the existing table in place and rehashes every
// entry. No new table and no new entries are created, entries are only relinked.
static void grow(ioopm_hash_table_t *ht)
{
    //Unlink every entry into a temporary list
    entry_t *all = NULL;
    for (size_t i = 0; i < ht->capacity; i++)
    {
        entry_t *current = ht->buckets[i];
        while (current != NULL)
        {
            entry_t *next = current->next;
            current->next = all;
            all = current;
            current = next;
        }
    }

    // Make the bucket array larger and mark every bucket empty
    size_t new_capacity = next_capacity(ht->capacity);
    ht->buckets = realloc(ht->buckets, new_capacity * sizeof(entry_t *));
    for (size_t i = 0; i < new_capacity; i++)
    {
        ht->buckets[i] = NULL;
    }
    ht->capacity = new_capacity;

    //Put every entry first in its new bucket
    while (all != NULL)
    {
        entry_t *next = all->next;
        size_t bucket = ht->hash_fn(all->key) % ht->capacity;
        all->next = ht->buckets[bucket];
        ht->buckets[bucket] = all;
        all = next;
    }
}

// Called early in operations that may add an entry, so they can proceed
// knowing there is room for one more.
static void grow_if_needed(ioopm_hash_table_t *ht)
{
    float load = (float)(ht->size + 1) / ht->capacity;
    if (load > ht->load_factor)
    {
        grow(ht);
    }
}

ioopm_hash_table_t *ioopm_hash_table_create_new(size_t initial_capacity, float load_factor, ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn)
{
    ioopm_hash_table_t *ht = malloc(sizeof(ioopm_hash_table_t));
    ht->hash_fn = hash_fn;
    ht->key_eq_fn = key_eq_fn;
    ht->load_factor = load_factor;
    ht->capacity = initial_capacity;
    ht->buckets = calloc(initial_capacity, sizeof(entry_t *)); // all buckets NULL
    ht->size = 0;
    return ht;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn)
{
    return ioopm_hash_table_create_new(Init_Capacity, Default_Load_Factor, hash_fn, key_eq_fn);
}

// The loop starts at .next because the sentinel nodes are built into the struct
// and are not malloc:ed.
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        entry_t *current = ht->buckets[i];

        while (current != NULL)
        {
            entry_t *next = current->next;
            entry_destroy(current);
            current = next;
        }
    }
    free(ht->buckets);
    free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    grow_if_needed(ht);

    // find previous entry, or the last entry if the key does not exist
    entry_t **link = find_entry_link(ht, key);

    // if the key exists, update the value, otherwise create a new entry
    if (*link != NULL)
    {
        (*link)->value = value;
    }
    else
    {
        *link = entry_create(key, value, NULL);
        ht->size++;
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
    // look for an entry with the key we want
    entry_t *entry = *find_entry_link(ht, key);

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (entry != NULL)
    {
        *result = entry->value;
        return true;
    }
    else
    {
        return false;
    }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
    entry_t **link = find_entry_link(ht, key);
    entry_t *current = *link;

    if (current == NULL)
    {
        return false;
    }
    else
    {
        *result = current->value;
        *link = current->next; // works the same for the first entry and the rest
        entry_destroy(current);
        ht->size--;
        return true;
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const elem_t key)
{
    elem_t trash;
    bool result = ioopm_hash_table_lookup(ht, key, &trash);
    return result;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
    if (ht->size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
    return ht->size;
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_iterator_t *it = malloc(sizeof(ioopm_hash_table_iterator_t));
    it->ht = ht;
    it->current_bucket = 0;
    find_next_non_empty_bucket(it);
    return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    free(it);
}

bool ioopm_hash_table_iterator_at_end(const ioopm_hash_table_iterator_t *it)
{
    if (it->current_entry == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    it->current_entry = it->current_entry->next;

    // end of this bucket, continue in the next non-empty one
    if (it->current_entry == NULL)
    {
        it->current_bucket++;
        find_next_non_empty_bucket(it);
    }
}

elem_t ioopm_hash_table_iterator_current_key(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->value;
}