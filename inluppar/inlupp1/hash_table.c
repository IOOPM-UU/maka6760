#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"

#define No_Buckets 17

struct entry
{
    elem_t key;     // holds the key
    elem_t value;     // holds the value
    entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
    size_t size;
    entry_t buckets[No_Buckets];
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

static entry_t *entry_destroy(entry_t *entry)
{
    free(entry);
    return NULL;
}

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  // advance to the next entry in the bucket
  it->current_entry = it->current_entry->next;

  // if it was null advance to the next bucket
  if (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    // if the next bucket existed, update the current entry
    if (it->current_bucket != No_Buckets)
    {
      it->current_entry = &it->ht->buckets[it->current_bucket];
     }
  }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket != No_Buckets &&
         it->current_entry == &it->ht->buckets[it->current_bucket])
  {
    advance_iterator_state(it); // Cheat!
  }
}

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, const elem_t key)
{
    size_t bucket = ht->hash_fn(key) % No_Buckets;
    entry_t *previous = &ht->buckets[bucket];
    entry_t *current = previous->next;
    while (current != NULL && !ht->key_eq_fn(current->key, key))
    {   
        previous = current;
        current = current->next;
    }
    return previous;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn)
{
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    ht->hash_fn = hash_fn;
    ht->key_eq_fn = key_eq_fn;
    ht->size = 0;
    return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < No_Buckets; i++)
    {
        entry_t *current = ht->buckets[i].next;

        while (current != NULL)
        {
            entry_t *next = current->next;
            entry_destroy(current);
            current = next;
        }
    }
    free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    // find previous entry, or the last entry if the key does not exist
    entry_t *previous = find_previous_entry(ht, key);

    // if the key exists, update the value, otherwise create a new entry
    if (previous->next != NULL)
    {
        previous->next->value = value;
    }
    else
    {
        previous->next = entry_create(key, value, NULL);
        ht->size++;
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
    // look for an entry with the key we want
    entry_t *previous = find_previous_entry(ht, key);

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (previous->next != NULL)
    {
        *result = previous->next->value;
        return true;
    }
    else
    {
        return false;
    }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *current = previous->next;

    if (current == NULL)
    {
        return false;
    }
    else
    {
        *result = current->value;
        entry_t *next_pointer = current->next;
        previous->next = next_pointer;
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
  it->current_entry = &ht->buckets[0];
  skip_sentinel_nodes(it);
  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    free(it);
}

bool ioopm_hash_table_iterator_at_end(const ioopm_hash_table_iterator_t *it)
{
    if (it->current_entry == NULL && it->current_bucket == No_Buckets) 
    {
        return true;
    } else
    {
        return false;
    }
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    advance_iterator_state(it);
    skip_sentinel_nodes(it);
}

elem_t ioopm_hash_table_iterator_current_key(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->key;

}

elem_t ioopm_hash_table_iterator_current_value(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->value;

}