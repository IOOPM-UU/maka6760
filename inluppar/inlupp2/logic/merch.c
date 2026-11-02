#include "merch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/**
 * NOTE: Some of the comments for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

struct merch 
{
    char *name;
    char *desc;
    int price;
    ioopm_list_t *storage;
};

struct shelf 
{
    char *shelf;
    size_t quantity;
};


/// @brief Creates a new shelf
static ioopm_shelf_t *create_shelf(char *shelf_name, size_t amount) 
{
    ioopm_shelf_t *new_shelf = malloc(sizeof(ioopm_shelf_t));
    new_shelf->shelf = shelf_name;
    new_shelf->quantity = amount;

    return new_shelf;
}

// Creates a new merch. The merch takes ownership of name and desc (no copy is made).
ioopm_merch_t *ioopm_merch_create(char *name, char *desc, int price)
{   
    ioopm_merch_t *new_merch = malloc(sizeof(ioopm_merch_t));
    new_merch->name = name;
    new_merch->desc = desc;
    new_merch->price = price;
    new_merch->storage = ioopm_list_create();

    return new_merch;
}

// Destroys a merch and the related storage list. Each shelf name is freed here,
// which is also the key used in storage_ht, so the merch must already be out of the store.
void ioopm_merch_destroy(ioopm_merch_t *merch) 
{
    ioopm_list_t *storage = merch->storage;

    while (!ioopm_list_is_empty(storage))
    {
        elem_t result;
        ioopm_shelf_t *current_shelf = ioopm_list_head(storage).p;
        free(current_shelf->shelf);
        ioopm_list_remove(storage, 0, &result);
        free(result.p);
    }

    ioopm_list_destroy(storage);
    free(merch->name);
    free(merch->desc);
    free(merch);
}

// Destroys all merch in a merch hash table and related storage hash table
void ioopm_merch_destroy_all(ioopm_webstore_t *store) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    ioopm_all_shelves_t *storage_ht = store->storage_ht;

	ioopm_hash_table_iterator_t *merch_iterator = ioopm_hash_table_iterator_create(merch_ht);
	while (!ioopm_hash_table_iterator_at_end(merch_iterator))
	{
		ioopm_merch_t *current_merch = ioopm_hash_table_iterator_current_value(merch_iterator).p;
		ioopm_merch_destroy(current_merch);
        ioopm_hash_table_iterator_advance(merch_iterator);
	}

	ioopm_hash_table_iterator_destroy(merch_iterator);

    ioopm_hash_table_iterator_t *storage_iterator = ioopm_hash_table_iterator_create(storage_ht);
    while (!ioopm_hash_table_iterator_at_end(storage_iterator))
    {
        free(ioopm_hash_table_iterator_current_value(storage_iterator).s);
        ioopm_hash_table_iterator_advance(storage_iterator);

    }
    ioopm_hash_table_iterator_destroy(storage_iterator);
}

// Returns the name of the given merch
char *ioopm_merch_get_name(ioopm_merch_t *merch) 
{
    return merch->name;
}

// Returns the description of the given merch
char *ioopm_merch_get_description(ioopm_merch_t *merch) 
{
    return merch->desc;
}

// Returns the price of the given merch
size_t ioopm_merch_get_price(ioopm_merch_t *merch) 
{
    return merch->price;
}

// Returns the storage list of the given merch
ioopm_list_t *ioopm_merch_get_storage_list(ioopm_merch_t *merch) 
{
    return merch->storage;
}

// Returns the name of the given shelf
char *ioopm_merch_get_shelf_name(ioopm_shelf_t *shelf) 
{
    return shelf->shelf;
}

// Returns the quantity of the merch on the given shelf
size_t ioopm_merch_get_shelf_quantity(ioopm_shelf_t *shelf) 
{
    return shelf->quantity;
}

// Returns the list of all storage locations for a particular merch
ioopm_list_t *ioopm_merch_get_stock(ioopm_webstore_t *store, char *merch) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    elem_t lookup_merch;

    if (!ioopm_hash_table_lookup(merch_ht, string_elem(merch), &lookup_merch))
    {
        return NULL;
    }
    ioopm_merch_t *looked_up_merch = lookup_merch.p;

    return looked_up_merch->storage;
}

// Add new merch to the merch hash table in the store
bool ioopm_merch_add(ioopm_webstore_t *store, ioopm_merch_t *new_merch) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    char *name = new_merch->name;

    if (ioopm_hash_table_has_key(merch_ht, string_elem(name)))
    {
        return false;
    }

    ioopm_hash_table_insert(merch_ht, string_elem(name), ptr_elem(new_merch));
    return true;
}

// Remove merch from the store
bool ioopm_merch_remove(ioopm_webstore_t *store, char *merch_remove, elem_t *result) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    ioopm_all_shelves_t *storage_ht = store->storage_ht;

    ioopm_list_t *storage = ioopm_merch_get_stock(store, merch_remove);
    if (!storage)
    {
        return false;
    }

    ioopm_list_iterator_t *storage_iterator = ioopm_list_iterator_create(storage);

    // Loops through the storage list to see what shelves are occupied by the specific merch,
    // and removes those shelves from the storage hash table
    elem_t removed_merch;
    while (!ioopm_list_iterator_at_end(storage_iterator))
    {
        ioopm_shelf_t *current_shelf = ioopm_list_iterator_current(storage_iterator).p;
        char *shelf_name = current_shelf->shelf;
        if (ioopm_hash_table_remove(storage_ht, string_elem(shelf_name), &removed_merch))
        {
            free(removed_merch.s); //frees the removed merch from storage_ht
        }
        ioopm_list_iterator_advance(storage_iterator);
    }
    ioopm_list_iterator_destroy(storage_iterator);

    return ioopm_hash_table_remove(merch_ht, string_elem(merch_remove), result);
}

// Returns the quantity of a merch in the hash table
size_t ioopm_merch_get_quantity(ioopm_webstore_t *store, char *name) 
{
    ioopm_list_t *storage = ioopm_merch_get_stock(store, name);
    if (!storage)
    {
        return 0;
    }

    size_t counter = 0;
    ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(storage);

    while (!ioopm_list_iterator_at_end(iterator))
    {
        ioopm_shelf_t *current_shelf = ioopm_list_iterator_current(iterator).p;

        size_t quantity = current_shelf->quantity;
        counter += quantity;
        ioopm_list_iterator_advance(iterator);
    }

    ioopm_list_iterator_destroy(iterator);

    return counter;
}

// Edits the name, description and price of a merch. Done by removing the old merch,
// moving its storage list over to new_merch and adding new_merch in its place, then
// registering the shelves in storage_ht again under the new name.
bool ioopm_merch_edit(ioopm_webstore_t *store, char *old_merch, ioopm_merch_t *new_merch) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    elem_t merch_to_edit;
    elem_t check_existance;
    
    // Stop the function if the merch to edit does not exist, or if the new merch has the same name as another merch
    if (!ioopm_hash_table_lookup(merch_ht, string_elem(old_merch), &merch_to_edit) || 
        (ioopm_hash_table_lookup(merch_ht, string_elem(new_merch->name), &check_existance) &&
        strcmp(old_merch, new_merch->name) != 0))
    {
        return false;
    }
    ioopm_merch_t *merch = merch_to_edit.p;

    // Remove the old merch
    ioopm_list_t *old_storage = merch->storage;
    elem_t merch_to_remove;
    if (!ioopm_merch_remove(store, merch->name, &merch_to_remove))
    {
        return false;
    }

    // Set the new storage as the old storage
    ioopm_list_t *new_empty_storage = new_merch->storage;
    ioopm_list_destroy(new_empty_storage);
    new_merch->storage = old_storage;
    
    ioopm_merch_add(store, new_merch);
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(new_merch->storage);
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *shelf = ioopm_list_iterator_current(it).p;
        ioopm_hash_table_insert(store->storage_ht,
                                string_elem(shelf->shelf),
                                string_elem(strdup(new_merch->name)));
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    ioopm_merch_t *old = merch_to_remove.p;
    free(old->name);
    free(old->desc);
    free(old);

    return true;
}


/// @brief Returns true if the shelf exists in the store
static bool check_shelf_existance(ioopm_webstore_t *store, char *shelf) 
{
    ioopm_all_shelves_t *storage_ht = store->storage_ht;

    if (ioopm_hash_table_has_key(storage_ht, string_elem(shelf)))
    {
        return true;
    }
    
    return false;
}

// Checks if a string is a shelf
bool ioopm_merch_is_shelf(char *shelf) 
{
    int length = strlen(shelf);
    if (length != 3)
    {
        return false;
    }

    if (!isupper(shelf[0]))
    {
        return false;
    }

    if (!isdigit(shelf[1]) || !isdigit(shelf[2]))
    {
        return false;
    }

    return true;
}

/// @brief Returns true if the given shelf contains the given merch
static bool check_merch_on_shelf(ioopm_webstore_t *store, char *shelf, char *merch) 
{
    ioopm_all_shelves_t *storage_ht = store->storage_ht;
    elem_t lookup_merch;

    if (!ioopm_hash_table_lookup(storage_ht, string_elem(shelf), &lookup_merch))
    {
        return false;
    }

    return strcmp(lookup_merch.s, merch) == 0;
}

// Increase the stock of a merch on an existent storage location or a new one
bool ioopm_merch_replenish(ioopm_webstore_t *store, char *merch, char *shelf, int amount) 
{
    ioopm_all_shelves_t *storage_ht = store->storage_ht;

    if (amount <= 0)
    {
        return false;
    }

    // Return false if another merch occupies the shelf
    bool shelf_exists = check_shelf_existance(store, shelf);
    bool merch_on_shelf = check_merch_on_shelf(store, shelf, merch);
    if (shelf_exists && !merch_on_shelf)
    {
        return false;
    }

    // Return false if the merch do not exist
    ioopm_list_t *storage = ioopm_merch_get_stock(store, merch);
    if (!storage)
    {
        return false;
    }

    // If the shelf does not already exist, insert it both in the storage list for the specific merch, 
    // and in the hash table with all the shelves in the webstore
    if (!shelf_exists)
    {   
        char *shelf_copy = strdup(shelf);
        char *merch_copy = strdup(merch);
        ioopm_list_append(storage, ptr_elem(create_shelf(shelf_copy, amount)));
        ioopm_hash_table_insert(storage_ht, string_elem(shelf_copy), string_elem(merch_copy));
        return true;
    }

    // Else, if the shelf exists for the given merch, increase the quantity by the given amount
    ioopm_list_iterator_t *storage_iterator = ioopm_list_iterator_create(storage);
    while (!ioopm_list_iterator_at_end(storage_iterator))
    {
        ioopm_shelf_t *current_shelf = ioopm_list_iterator_current(storage_iterator).p;

        if (strcmp(current_shelf->shelf, shelf) == 0)
        {
            current_shelf->quantity += amount;
            ioopm_list_iterator_destroy(storage_iterator);
            return true;
        }

        ioopm_list_iterator_advance(storage_iterator);
    }
    ioopm_list_iterator_destroy(storage_iterator);

    return false;
}


// Decrease the stock of a merch on an existent storage location
bool ioopm_merch_decrease_stock(ioopm_webstore_t *store, char *merch, char *shelf, int amount) 
{
    ioopm_all_shelves_t *storage_ht = store->storage_ht;

    if (amount <= 0)
    {
        return false;
    }

    // Return false if the given shelf is not occupied by the given merch
    if (!check_merch_on_shelf(store, shelf, merch))
    {
        return false;
    }

    ioopm_list_t *storage = ioopm_merch_get_stock(store, merch);
    ioopm_list_iterator_t *storage_iterator = ioopm_list_iterator_create(storage);
    ioopm_shelf_t *current_shelf = ioopm_list_iterator_current(storage_iterator).p;

    while (strcmp(current_shelf->shelf, shelf) != 0)
    {
        ioopm_list_iterator_advance(storage_iterator);
        current_shelf = ioopm_list_iterator_current(storage_iterator).p;
    }

    int current_amount = current_shelf->quantity;

    if (amount < current_amount)
    {
        current_shelf->quantity -= amount;
    }
    else
    {        
        elem_t result;
        ioopm_hash_table_remove(storage_ht, string_elem(shelf), &result);
        ioopm_list_iterator_remove(storage_iterator);
        free(result.s); //frees the value stored in storage_ht
        free(current_shelf->shelf);
        free(current_shelf);
    }

    ioopm_list_iterator_destroy(storage_iterator);

    return true;
}