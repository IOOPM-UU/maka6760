#include "cart.h"
#include "webstore.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * NOTE: Some of the comments for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

int ioopm_cart_add_new(ioopm_webstore_t *store, ioopm_hash_function *hash_fn,
                 ioopm_eq_function *key_eq_fn)
{
    ioopm_cart_t *new_cart = ioopm_hash_table_create(hash_fn, key_eq_fn);
    int cart_num = store->next_cart_num;
    ioopm_hash_table_insert(store->cart_ht, int_elem(cart_num), ptr_elem(new_cart));
    store->next_cart_num++;
    return cart_num;
}

/// @brief Frees the keys of a cart. The keys are copies owned by the cart.
static void free_cart_keys(ioopm_cart_t *cart)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        free(ioopm_hash_table_iterator_current_key(it).s);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
}

bool ioopm_cart_destroy(ioopm_webstore_t *store, int cart_num)
{
    elem_t removed;
    if (!ioopm_hash_table_remove(store->cart_ht, int_elem(cart_num), &removed))
    {
        return false;
    }
    free_cart_keys(removed.p);
    ioopm_hash_table_destroy(removed.p);
    return true;
}

void ioopm_cart_destroy_all(ioopm_webstore_t *store)
{
    ioopm_hash_table_iterator_t *cart_iterator = ioopm_hash_table_iterator_create(store->cart_ht);
    while (!ioopm_hash_table_iterator_at_end(cart_iterator)) 
    {
        ioopm_cart_t *current_cart = ioopm_hash_table_iterator_current_value(cart_iterator).p;
        free_cart_keys(current_cart);
        ioopm_hash_table_destroy(current_cart);
        ioopm_hash_table_iterator_advance(cart_iterator);
    }
    ioopm_hash_table_iterator_destroy(cart_iterator);
}

static size_t reserved_in_carts(ioopm_webstore_t *store, char *name)
{
    size_t reserved = 0;
    ioopm_hash_table_iterator_t *carts = ioopm_hash_table_iterator_create(store->cart_ht);
    while (!ioopm_hash_table_iterator_at_end(carts))
    {
        ioopm_cart_t *cart = ioopm_hash_table_iterator_current_value(carts).p;
        elem_t in_cart;
        if (ioopm_hash_table_lookup(cart, string_elem(name), &in_cart))
        {
            reserved += (size_t)in_cart.i;
        }
        ioopm_hash_table_iterator_advance(carts);
    }
    ioopm_hash_table_iterator_destroy(carts);
    return reserved;
}

/// @brief A helper that checks if the cart exists, if it does it returns the pointer to the cart
static ioopm_cart_t *get_cart(ioopm_webstore_t *store, int cart_num)
{
    elem_t current; 
    if(!ioopm_hash_table_lookup(store->cart_ht, int_elem(cart_num), &current))
    {
        return NULL;
    }
    return current.p;
}


bool ioopm_cart_add_item(ioopm_webstore_t *store, int cart_num, char *name, int amount)
{
    ioopm_cart_t *current_cart = get_cart(store, cart_num);
    if(current_cart == NULL)
    {
        return false;
    }

    if (amount <= 0)
    {
        return false;
    }

    if (ioopm_merch_get_quantity(store, name) < reserved_in_carts(store, name) + (size_t)amount)
    {
        return false;
    }

    int in_cart = 0;
    elem_t existing;
    if (ioopm_hash_table_lookup(current_cart, string_elem(name), &existing))
    {
        in_cart = existing.i;
        ioopm_hash_table_insert(current_cart, string_elem(name), int_elem(in_cart + amount));
    }
    else
    {
        ioopm_hash_table_insert(current_cart, string_elem(strdup(name)), int_elem(in_cart + amount));
    }

    return true;
}

bool ioopm_cart_remove_item(ioopm_webstore_t *store, int cart_num, char *name, int amount)
{
    ioopm_cart_t *current_cart = get_cart(store, cart_num);
    if (current_cart == NULL)
    {
        return false;
    }
    
    if (amount < 0)
    {
        return false;
    }
    
    elem_t existing;
    if (!ioopm_hash_table_lookup(current_cart, string_elem(name), &existing))
    {
        return false;
    }
    
    if (amount > existing.i)
    {
        return false;
    }

    int new_amount = existing.i - amount;
    if (new_amount == 0)
    {
        // The key is our own copy of the name. It has to be found before the
        // entry is removed, since the table does not hand the key back.
        char *key = NULL;
        ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(current_cart);
        while (!ioopm_hash_table_iterator_at_end(it))
        {
            char *k = ioopm_hash_table_iterator_current_key(it).s;
            if (strcmp(k, name) == 0)
            {
                key = k;
            }
            ioopm_hash_table_iterator_advance(it);
        }
        ioopm_hash_table_iterator_destroy(it);

        elem_t removed;
        ioopm_hash_table_remove(current_cart, string_elem(name), &removed);
        free(key);
    }
    else
    {
        ioopm_hash_table_insert(current_cart, string_elem(name), int_elem(new_amount));
    }
    return true;
}

int ioopm_cart_calculate_cost(ioopm_webstore_t *store, int cart_num)
{
    ioopm_cart_t *current_cart = get_cart(store, cart_num);
    if (current_cart == NULL)
    {
        return -1;
    }

    int sum = 0;
    ioopm_hash_table_iterator_t *iterator = ioopm_hash_table_iterator_create(current_cart);
    while (!ioopm_hash_table_iterator_at_end(iterator))
    {
        elem_t name = ioopm_hash_table_iterator_current_key(iterator);
        elem_t amount = ioopm_hash_table_iterator_current_value(iterator);

        elem_t merch;
        bool found = ioopm_hash_table_lookup(store->merch_ht, name, &merch);
        assert(found);
        (void)found; // quiets unused variable-warning

        ioopm_merch_t *current_merch = merch.p;
        sum += ioopm_merch_get_price(current_merch) * amount.i;

        ioopm_hash_table_iterator_advance(iterator);
    }
    ioopm_hash_table_iterator_destroy(iterator);
    return sum;
}

/// @brief Takes amount of a merch from its shelves, one shelf at a time.
///        PRE: the merch exists and total stock >= amount (checked in checkout).
static void take_from_shelves(ioopm_webstore_t *store, char *name, size_t amount)
{
    //checks that the merch exists, if not it will crash the program
    elem_t lookup_merch;
    bool found = ioopm_hash_table_lookup(store->merch_ht, string_elem(name), &lookup_merch);
    assert(found);
    (void)found;

    ioopm_merch_t *merch = lookup_merch.p;
    size_t remaining = amount;

    while (remaining > 0)
    {
        assert(!ioopm_list_is_empty(ioopm_merch_get_storage_list(merch)));
        ioopm_shelf_t *first = ioopm_list_head(ioopm_merch_get_storage_list(merch)).p;

        char *shelf_name = ioopm_merch_get_shelf_name(first);
        size_t quantity = ioopm_merch_get_shelf_quantity(first);
        size_t take = remaining < quantity ? remaining : quantity;

        ioopm_merch_decrease_stock(store, name, shelf_name, take);
        remaining -= take;
    }
}

bool ioopm_cart_checkout(ioopm_webstore_t *store, int cart_num)
{
    elem_t current;
    if (!ioopm_hash_table_lookup(store->cart_ht, int_elem(cart_num), &current))
    {
        return false;
    }
    ioopm_cart_t *current_cart = current.p;

    // This loop checks that there is enough stock for every merch in the cart,
    //  if not, it returns false, and nothing changes.
    ioopm_hash_table_iterator_t *iterator = ioopm_hash_table_iterator_create(current_cart);
    while (!ioopm_hash_table_iterator_at_end(iterator))
    {
        elem_t name = ioopm_hash_table_iterator_current_key(iterator);
        elem_t amount = ioopm_hash_table_iterator_current_value(iterator);
        char *current_name = name.s;
        if (ioopm_merch_get_quantity(store, current_name) < (size_t)amount.i)
        {
            ioopm_hash_table_iterator_destroy(iterator);
            return false;
        }
        ioopm_hash_table_iterator_advance(iterator);
    }
    ioopm_hash_table_iterator_destroy(iterator);

    // This loop will only run if there is enough stock for every item in the cart
    ioopm_hash_table_iterator_t *iterator2 = ioopm_hash_table_iterator_create(current_cart);
    while (!ioopm_hash_table_iterator_at_end(iterator2))
    {
        elem_t name = ioopm_hash_table_iterator_current_key(iterator2);
        elem_t amount = ioopm_hash_table_iterator_current_value(iterator2);
        char *current_name = name.s;

        take_from_shelves(store, current_name, (size_t)amount.i);
        ioopm_hash_table_iterator_advance(iterator2);
    }
    ioopm_hash_table_iterator_destroy(iterator2);
    return ioopm_cart_destroy(store, cart_num); // always returns true, checkout will never run on a non-existing cart
}