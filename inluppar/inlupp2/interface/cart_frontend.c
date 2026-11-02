#include "cart_frontend.h"
#include "cart.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "hash_funcs.h"

/**
 * NOTE: Some of the comments for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

// Front end function for creating the cart
void ioopm_front_cart_add_new(ioopm_webstore_t *store)
{
    int cart_num = ioopm_cart_add_new(store, string_hash, string_eq);
    printf("Cart %d created, time to start shopping!\n", cart_num);
}

// Front end function for removing cart
void ioopm_front_cart_remove(ioopm_webstore_t *store)
{
    int cart_num = ask_question_int("Which cart do you want to remove?");
    char *confirmation = ask_question_string("Are you sure you want to remove this cart? (Y/N)");
    bool confirmed = toupper(confirmation[0]) == 'Y';
    free(confirmation);
    if (!confirmed)
    {
        printf("Returning to menu\n");
    }
    else
    {
        if (ioopm_cart_destroy(store, cart_num))
        {
            printf("%d was successfully removed\n", cart_num);
        }
        else
        {
            printf("%d is not a valid cart or does not exist.\n", cart_num);
        }
    }
}

// Front end function for adding to cart
void ioopm_front_cart_add_item(ioopm_webstore_t *store)
{
    int cart_num = ask_question_int("Enter a valid cart number: ");
    char *name = ask_question_string("What's the item you want to add?");
    int amount = ask_question_int("How many of the item would you like to add?");

    if (ioopm_cart_add_item(store, cart_num, name, amount))
    {
        printf("You've added %d x %s to cart #%d.\n", amount, name, cart_num);
    }
    else
    {
        printf("Unable to add: check that the cart and item exist and that there is enough stock.\n");
    }
    free(name);
}

// Front end function for removing from cart
void ioopm_front_cart_remove_item(ioopm_webstore_t *store)
{
    int cart_num = ask_question_int("Enter a valid cart number: ");
    char *name = ask_question_string("What's the item you want to remove?");
    int amount = ask_question_int("How many of the item would you like to remove?");

    if (ioopm_cart_remove_item(store, cart_num, name, amount))
    {
        printf("You've removed %d x %s from cart #%d.\n", amount, name, cart_num);
    }
    else
    {
        printf("Unable to remove: check that the cart exists and that it contains at least that many of the item.\n");
    }
    free(name);
}

// Front end function for calculating cost of cart
void ioopm_front_cart_calculate_cost(ioopm_webstore_t *store)
{
    int cart_num = ask_question_int("Enter a valid cart number: ");
    int sum = ioopm_cart_calculate_cost(store, cart_num);
    if (sum == -1)
    {
        printf("%d is not a valid cart or does not exist.\n", cart_num);
    }
    else
    {
        printf("The total cost of cart #%d is %d kr.\n", cart_num, sum);
    }
}

// Front end function for checking out a cart
void ioopm_front_cart_checkout(ioopm_webstore_t *store)
{
    int cart_num = ask_question_int("Enter a valid cart number: ");
    int sum = ioopm_cart_calculate_cost(store, cart_num);
    // calculate_cost returns -1 if the cart is invalid.
    if (sum == -1)
    {
        printf("Cart #%d does not exist.\n", cart_num);
    }
    else if (ioopm_cart_checkout(store, cart_num))
    {
        printf("Purchase complete! Total: %d kr.\n", sum);
    }
    else
    {
        printf("Not enough stock to complete to the purchase of cart #%d.\n", cart_num);
    }
}