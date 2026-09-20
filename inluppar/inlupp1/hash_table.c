#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "/home/lillmacke/IOPM/inluppar/inlupp1/hash_table.h"

#define No_Buckets 17

struct entry
{
  char *key;     // holds the key
  int value;     // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  int size;
  entry_t buckets[No_Buckets];
};

static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }
  return result;
}

static entry_t *entry_create(char *key, int value, entry_t *next)
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

ioopm_hash_table_t *ioopm_hash_table_create()
{
  // Allocate zeroed out space for a ioopm_hash_table_t = No_Buckets pointers to entry_t's
  return calloc(1, sizeof(ioopm_hash_table_t));
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  // TODO: Stub
  for (int i = 0; i < No_Buckets; i++)
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

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key)
{
  // find bucket
  size_t bucket = string_knr_hash(key) % No_Buckets;

  // look for an entry with the key we want
  entry_t *previous = &ht->buckets[bucket];
  entry_t *current = previous->next;
  while (current != NULL && strcmp(current->key, key) != 0)
  {
    current = current->next;
  }
  return previous;
}
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value)
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

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result)
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

int ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key)
{
  entry_t *previous = find_previous_entry(ht, key);
  entry_t *current = previous->next;

  if (current == NULL)
  {
    printf("%s does not exist\n", key);
    return -1;
  }
  else
  {
    int result = current->value;
    entry_t *next_pointer = current->next;
    previous->next = next_pointer;
    entry_destroy(current);
    ht->size--;
    return result;
  }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, char *key)
{
  int trash;
  bool result = ioopm_hash_table_lookup(ht, key, &trash);
  return result;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  if (ht->size == 0)
  {
    return true;
  } else 
  {
    return false;
  }
}

int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{ 
  return ht->size;
}