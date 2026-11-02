#pragma once
#include "linked_list.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "merch.h"

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/**
* @file cart.h
* @author Markus Karlsson and Alva Fällman Nilsson
* @brief Shopping carts of the webstore.
*
* A cart is a hash table that maps merch name (string) => amount (int). Carts
* are stored in the store's cart_ht and are identified by a cart number, which
* is handed out by ioopm_cart_add_new. Numbers start at 1 and are never reused.
*
* Putting merch in a cart reserves it: the amount in all carts together can
* never exceed the stock. Stock is not decreased until the cart is checked out.
*
* The cart keeps its own copies of the merch names, so the names passed to
* these functions do not need to outlive the call.
*/

/// @brief Represents a cart in the store. 
typedef ioopm_hash_table_t ioopm_cart_t;

/// @brief Add a new empty cart to the store
/// @param store the store to add the cart to
/// @param hash_fn hash function for the merch names in the cart (string_hash)
/// @param key_eq_fn equality function for the merch names in the cart (string_eq)
/// @return the number of the new cart
int ioopm_cart_add_new(ioopm_webstore_t *store, ioopm_hash_function *hash_fn,
                 ioopm_eq_function *key_eq_fn);

/// @brief Remove a cart from the store and free it. The stock is not affected
/// @param store the store the cart belongs to
/// @param cart_num number of the cart to remove
/// @return true if the cart existed and was removed, false otherwise
bool ioopm_cart_destroy(ioopm_webstore_t *store, int cart_num);

/// @brief Add an amount of a merch to a cart. If the merch is already in the
///        cart, the amount is increased
/// @param store the store the cart belongs to
/// @param cart_num number of the cart
/// @param name name of the merch to add
/// @param amount how many to add
/// @return true if added. False, with the cart left unchanged, if the cart does
///         not exist, amount is not positive, or the amount in all carts together
///         would exceed the stock (which includes the merch not existing)
bool ioopm_cart_add_item(ioopm_webstore_t *store, int cart_num, char *name, int amount);

/// @brief Remove an amount of a merch from a cart. When the amount in the cart
///        reaches 0 the merch is removed from the cart altogether
/// @param store the store the cart belongs to
/// @param cart_num number of the cart
/// @param name name of the merch to remove
/// @param amount how many to remove
/// @return true if removed. False, with the cart left unchanged, if the cart does
///         not exist, amount is negative, the merch is not in the cart, or amount
///         is larger than what is in the cart
bool ioopm_cart_remove_item(ioopm_webstore_t *store, int cart_num, char *name, int amount);

/// @brief Free every cart in the store (the carts and the merch names they own)
/// @note The entries in cart_ht are not removed, so the dangling pointers must
///       not be used. Call this only when shutting down, followed by ioopm_webstore_destroy
/// @param store the store whose carts are freed
void ioopm_cart_destroy_all(ioopm_webstore_t *store);

/// @brief Calculate the total price of everything in a cart
/// @pre every merch in the cart still exists in the store (it is asserted)
/// @param store the store the cart belongs to
/// @param cart_number number of the cart
/// @return the sum of price * amount over the cart, or -1 if the cart does not exist
int ioopm_cart_calculate_cost(ioopm_webstore_t *store, int cart_number);

/// @brief Buy everything in a cart: decrease the stock of every merch in it by
///        the amount in the cart, then remove the cart.
///        Stock is taken from the merch's shelves in the order they are listed,
///        and a shelf that becomes empty is removed
/// @param store the store the cart belongs to
/// @param cart_num number of the cart
/// @return true if the checkout succeeded. False, with nothing changed, if the
///         cart does not exist or the stock of some merch in it is too small
bool ioopm_cart_checkout(ioopm_webstore_t *store, int cart_num);
