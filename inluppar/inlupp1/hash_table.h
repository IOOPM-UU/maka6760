#pragma once
#include <stdbool.h>

typedef struct entry entry_t;
/**
* @file hash_table.h
* @author Markus Karlsson & Vilgot Lenninger
* @date 14/9-2026
* @brief Simple hash table that maps string keys to integer values.
*
* Here typically goes a more extensive explanation of what the header
* defines. Doxygens tags are words preceeded by either a backslash @\
* or by an at symbol @@.
*
*/

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(void);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result if lookup succeeds, write resulting value to memory location
///               result points to.
/// @return true, if lookup succeeds (FIXME: what if the key does not exist?)
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key or -1 if the key doesn't exist.
/// NOTE: Because of this, -1 can NOT be stored as a value in the table. 
int ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key);