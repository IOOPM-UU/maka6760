#pragma once
#include "linked_list.h"
#include "list_iterator.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "webstore.h"
#include "utils.h"

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/**
* @file merch.h
* @author Markus Karlsson and Alva Fällman Nilsson
* @brief Merchandise and its storage in the webstore.
*
* A merch has a name (unique in the store), a description, a price and a list
* of the shelves it is stored on. A shelf has a name and a quantity. A shelf
* name is one capital letter followed by two digits, e.g. "A01" (see
* ioopm_merch_is_shelf), and a shelf holds at most one kind of merch.
*
* The merch in a store is kept in store->merch_ht, and which merch occupies
* which shelf is also kept in store->storage_ht. The functions here keep the
* two consistent, so use them instead of changing the tables directly.
*
* Ownership: a merch owns its name and description, so these must be
* heap-allocated (e.g. with strdup) and are freed by ioopm_merch_destroy.
* Once a merch has been added to a store with ioopm_merch_add, it belongs
* to the store.
*/


/// @brief Represents a merchandise item in the store
typedef struct merch ioopm_merch_t;

/// @brief Represents a shelf in the store's storage system.
typedef struct shelf ioopm_shelf_t;


/// @brief Create a new merch with an empty storage list. It is not part of any
///        store until it is added with ioopm_merch_add
/// @param name name of the merch. Not copied: the merch takes ownership, so it must be heap-allocated
/// @param desc description of the merch. Not copied: the merch takes ownership, so it must be heap-allocated
/// @param price price of the merch
/// @return the new merch
ioopm_merch_t *ioopm_merch_create(char *name, char *desc, int price);

/// @brief Destroy a merch: free its name, description, shelves and storage list
/// @param merch the merch to destroy
void ioopm_merch_destroy(ioopm_merch_t *merch);

/// @brief Destroy every merch in the store, and free the merch names stored in storage_ht
/// @note The entries in the tables are not removed, so the dangling pointers must
///       not be used. Call this only when shutting down, followed by ioopm_webstore_destroy
/// @param store the store whose merch is destroyed
void ioopm_merch_destroy_all(ioopm_webstore_t *store);


/// @brief Get the name of a merch
/// @param merch the merch
/// @return the name, owned by the merch
char *ioopm_merch_get_name(ioopm_merch_t *merch);

/// @brief Get the description of a merch
/// @param merch the merch
/// @return the description, owned by the merch
char *ioopm_merch_get_description(ioopm_merch_t *merch);

/// @brief Get the price of a merch
/// @param merch the merch
/// @return the price
size_t ioopm_merch_get_price(ioopm_merch_t *merch);

/// @brief Get the storage list of a merch
/// @param merch the merch
/// @return the list of its shelves (ioopm_shelf_t *, stored in the .p field),
///         owned by the merch. Do not modify or destroy it
ioopm_list_t *ioopm_merch_get_storage_list(ioopm_merch_t *merch);

/// @brief Get the name of a shelf
/// @param shelf the shelf
/// @return the name of the shelf, owned by the shelf
char *ioopm_merch_get_shelf_name(ioopm_shelf_t *shelf);

/// @brief Get the quantity of merch on a shelf
/// @param shelf the shelf
/// @return the number of items on the shelf
size_t ioopm_merch_get_shelf_quantity(ioopm_shelf_t *shelf);


/// @brief Get the shelves a merch is stored on
/// @param store the store
/// @param merch name of the merch
/// @return the storage list of the merch (see ioopm_merch_get_storage_list), or
///         NULL if there is no merch with that name
ioopm_list_t *ioopm_merch_get_stock(ioopm_webstore_t *store, char *merch);

/// @brief Add a merch to the store
/// @param store the store
/// @param new_merch the merch to add. If this succeeds the store owns it
/// @return true if added, false if a merch with the same name already exists
///         (the caller then still owns new_merch)
bool ioopm_merch_add(ioopm_webstore_t *store, ioopm_merch_t *new_merch);

/// @brief Remove a merch from the store, and free its shelves in storage_ht
///        so they become available to other merch
/// @param store the store
/// @param merch_remove name of the merch to remove
/// @param result where the removed merch (ioopm_merch_t *, in the .p field) is
///               written, if it existed. The caller must destroy it with
///               ioopm_merch_destroy. Left untouched otherwise
/// @return true if the merch existed and was removed, false otherwise
bool ioopm_merch_remove(ioopm_webstore_t *store, char *merch_remove, elem_t *result);

/// @brief Get the total quantity of a merch, summed over all its shelves
/// @param store the store
/// @param name name of the merch
/// @return the quantity, or 0 if there is no merch with that name
size_t ioopm_merch_get_quantity(ioopm_webstore_t *store, char *name);

/// @brief Replace the name, description and price of a merch. The shelves and
///        stock are kept
/// @note Carts are not updated, so a cart that holds the merch under its old
///       name keeps referring to that name
/// @param store the store
/// @param old_merch name of the merch to edit
/// @param new_merch a merch holding the new name, description and price (it may
///                  keep the old name). Its storage list must be empty. If this
///                  succeeds the store owns it
/// @return true if edited. False if old_merch does not exist or the new name
///         belongs to another merch (the caller then still owns new_merch)
bool ioopm_merch_edit(ioopm_webstore_t *store, char *old_merch, ioopm_merch_t *new_merch);

/// @brief Check if a string is a valid shelf name: one capital letter followed by two digits
/// @param shelf the string to check
/// @return true if it is a valid shelf name
bool ioopm_merch_is_shelf(char *shelf);

/// @brief Increase the stock of a merch on a shelf. If the shelf is not used yet
///        it is taken by the merch and is put into the storage hash table, 
///        otherwise the quantity on it is increased
/// @param store the store
/// @param merch name of the merch
/// @param shelf name of the shelf (the strings are copied)
/// @param amount how many to add
/// @return true if the stock was increased. False, with nothing changed, if amount
///         is not positive, the merch does not exist, or the shelf holds another merch
bool ioopm_merch_replenish(ioopm_webstore_t *store, char *merch, char *shelf, int amount);

/// @brief Decrease the stock of a merch on a shelf. If amount is as large as the
///        quantity on the shelf (or larger), the shelf is emptied and removed
///        from the merch and from storage_ht
/// @note Callers are expected to check that there is enough stock first.
///       Amounts larger than the quantity on the shelf are not an error
/// @param store the store
/// @param merch name of the merch
/// @param shelf name of the shelf
/// @param amount how many to take
/// @return true if the stock was decreased. False, with nothing changed, if amount
///         is not positive or the shelf does not hold that merch (which includes
///         the merch or the shelf not existing)
bool ioopm_merch_decrease_stock(ioopm_webstore_t *store, char *merch, char *shelf, int amount);
