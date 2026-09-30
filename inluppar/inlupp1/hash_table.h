#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

/**
* @file hash_table.h
* @author Markus Karlsson & Vilgot Lenninger
* @date 14/9-2026
* @brief Simple hash table that maps elem_t keys to elem_t values.
*
* Keys and values are stored as elem_t (see common.h), so the table can hold
* ints, unsigned ints, bools, floats, strings or arbitrary pointers. The caller
* is responsible for knowing which field of the union a key or value was
* stored in.
*
* Since a union carries no type information, the table cannot hash or compare
* keys on its own. Instead the caller passes a hash function and a key
* equality function to ioopm_hash_table_create, and the table uses only these
* to decide which bucket a key belongs in and to find it within the bucket.
*
* The table does not copy keys or values. Any memory that keys or values
* point to is owned by the caller, must outlive its mapping, and is never
* freed by the table.
*
*/
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @param hash_fn function that computes a hash value for a key. Used to decide
///                which bucket a key belongs in
/// @param key_eq_fn function that returns true if two keys are equal. Used to
///                  find the right entry within a bucket
/// @pre keys that key_eq_fn considers equal must get the same value from hash_fn,
///      otherwise they may end up in different buckets and not be found
/// @pre key_eq_fn and hash_fn must read the same field of the union as the keys
///      are stored in (e.g. .s for string keys, .i for int keys)
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn);

/// @brief Delete a hash table and free its memory (but not the memory of keys or values)
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht. If an equal key (according to
///        key_eq_fn) already exists, its value is replaced and the original key is kept
/// @param ht hash table operated upon
/// @param key key to insert. Not copied, so anything it points to must stay valid
///            while the mapping exists
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result if lookup succeeds, write resulting value to memory location
///               result points to. Left untouched if the key does not exist
/// @return true if the key exists and result was written, false otherwise
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @param result where the removed value is written, if the key existed. Left untouched otherwise
/// @return true if the key existed and result was written, false otherwise
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key, elem_t *result);

/// @brief (AI) check if a mapping for key exists in hash table ht
/// @param ht hash table operated upon
/// @param key key to check for 
/// @return true if ht contains mapping for key, false otherwise
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const elem_t key);

/// @brief (AI) check if hash table ht contains any mappings at all
/// @param ht hash table operated upon
/// @return true if ht contains no mappings, false otherwise
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief  (AI) count the number of mappings currently stored in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value mappings in ht, as a size_t (never negative)
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);