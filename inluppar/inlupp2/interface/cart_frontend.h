#pragma once
#include "cart.h"


/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/**
* @file cart_frontend.h
* @author Markus Karlsson and Alva Fällman Nilsson
* @brief The terminal user interface for carts.
*
* Each function is one entry in the main menu of the program: it asks the
* user for what it needs on stdin, calls the matching backend function in
* cart.h and prints the result. The functions never fail; problems such as an
* unknown cart number are reported to the user as a message.
*/

/// @brief Menu action: create a new cart and print its number
/// @param store the store to add the cart to
void ioopm_front_cart_add_new(ioopm_webstore_t *store);

/// @brief Menu action: ask for a cart number and, after confirmation (Y), remove that cart
/// @param store the store the cart belongs to
void ioopm_front_cart_remove(ioopm_webstore_t *store);

/// @brief Menu action: ask for a cart number, a merch name and an amount, and add that to the cart.
///        Prints a message if the cart or merch does not exist or the stock is too small
/// @param store the store the cart belongs to
void ioopm_front_cart_add_item(ioopm_webstore_t *store);

/// @brief Menu action: ask for a cart number, a merch name and an amount, and remove that from the cart.
///        Prints a message if the cart does not hold that much of the merch
/// @param store the store the cart belongs to
void ioopm_front_cart_remove_item(ioopm_webstore_t *store);

/// @brief Menu action: ask for a cart number and print the total cost of the cart
/// @param store the store the cart belongs to
void ioopm_front_cart_calculate_cost(ioopm_webstore_t *store);

/// @brief Menu action: ask for a cart number and check it out, printing the total
///        cost if the purchase succeeds, or a message if the stock is too small
/// @param store the store the cart belongs to
void ioopm_front_cart_checkout(ioopm_webstore_t *store);
