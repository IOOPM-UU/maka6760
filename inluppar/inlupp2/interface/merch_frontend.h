#pragma once
#include "merch.h"
#include "utils.h"

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/**
* @file merch_frontend.h
* @author Markus Karlsson and Alva Fällman Nilsson
* @brief The terminal user interface for merch and stock.
*
* Each function is one entry in the main menu of the program: it asks the
* user for what it needs on stdin, calls the matching backend function in
* merch.h and prints the result. The functions never fail; problems such as an
* unknown merch name are reported to the user as a message.
*/


/// @brief Menu action: ask for a name, description and price (not negative) and add a new merch.
///        Prints a message if a merch with that name already exists
/// @param store the store to add the merch to
void ioopm_front_merch_add(ioopm_webstore_t *store);

/// @brief Menu action: list the names of all merch, numbered, 20 at a time.
///        After every 20 names the user is asked if the listing should continue (N stops it).
///        The order is not sorted
/// @param store the store to list
void ioopm_front_merch_list(ioopm_webstore_t *store);

/// @brief Menu action: ask for the name of a merch and, after confirmation (Y), remove it
/// @param store the store the merch belongs to
void ioopm_front_merch_remove(ioopm_webstore_t *store);

/// @brief Menu action: ask for the name of an existing merch. Then ask for a new name, 
///        description and price and, after confirmation (Y), replace the old ones. The stock is kept.
///        Prints a message if the merch does not exist or the new name is already taken
/// @param store the store the merch belongs to
void ioopm_front_merch_edit(ioopm_webstore_t *store);

/// @brief Menu action: ask for the name of a merch and print the shelves it is
///        stored on, with the quantity on each
/// @param store the store the merch belongs to
void ioopm_front_merch_show_stock(ioopm_webstore_t *store);

/// @brief Menu action: ask for a shelf (a capital letter and two digits), a merch name and
///        an amount, and increase the stock of the merch on that shelf.
///        Prints a message if the merch does not exist or the shelf holds another merch
/// @param store the store the merch belongs to
void ioopm_front_merch_replenish(ioopm_webstore_t *store);

