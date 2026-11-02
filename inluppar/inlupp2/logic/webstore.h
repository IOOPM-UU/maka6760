#pragma once
#include "hash_table.h"
#include "common.h"

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/**
* @file webstore.h
* @author Markus Karlsson and Alva Fällman Nilsson
* @brief The webstore: the single object that holds all backend state.
*
* A webstore consists of three hash tables, which merch.h and cart.h operate on:
*  - merch_ht:   merch name (string) => ioopm_merch_t *
*  - cart_ht:    cart number (int)   => ioopm_cart_t *
*  - storage_ht: shelf name (string) => name of the merch stored on that shelf (string)
*
* Create the store with ioopm_webstore_create when the program starts and
* pass a pointer to it to every backend function.
*/

/// @brief Represents the hash table of all merch in the store.
typedef ioopm_hash_table_t ioopm_all_merch_t;

/// @brief Represents the hash table of all carts in the store.
typedef ioopm_hash_table_t ioopm_all_carts_t;

/// @brief Represents the hash table of all shelves in the store.
typedef ioopm_hash_table_t ioopm_all_shelves_t;

/// @brief The state of a webstore.
///        next_cart_num is the number the next cart will get, and is used as
///        key in cart_ht to keep track of individual carts
typedef struct webstore
{
    ioopm_all_merch_t *merch_ht;
    ioopm_all_carts_t *cart_ht;
    ioopm_all_shelves_t *storage_ht;
    int next_cart_num;
} ioopm_webstore_t;


/// @brief Create a webstore with three empty hash tables
/// @param string_hash hash function for string keys (merch names and shelf names)
/// @param string_eq equality function for string keys
/// @param int_hash hash function for int keys (cart numbers)
/// @param int_eq equality function for int keys
/// @return a new webstore with no merch and no carts. The first cart will get number 1
ioopm_webstore_t ioopm_webstore_create(ioopm_hash_function *string_hash, ioopm_eq_function *string_eq, ioopm_hash_function *int_hash, ioopm_eq_function *int_eq);

/// @brief Destroy the three hash tables of the store (but not what is stored in them)
/// @pre everything stored in the tables has already been freed, e.g. with
///      ioopm_merch_destroy_all and ioopm_cart_destroy_all, otherwise it leaks
/// @param store the store to be destroyed. It must not be used afterwards
void ioopm_webstore_destroy(ioopm_webstore_t *store);
