#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "webstore.h"
#include "cart_frontend.h"
#include "merch_frontend.h"
#include "common.h"
#include "hash_funcs.h"

/**
 * NOTE: Some of the comments for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/// @brief Prints the main menu, one line per action.
static void print_menu(void)
{
    puts("[A] Add merchandise\n"
         "[L] List merchandise\n"
         "[D] Delete merchandise\n"
         "[E] Edit merchandise\n"
         "[S] Show stock\n"
         "[P] Replenish\n"
         "[C] Create cart\n"
         "[R] Remove cart\n"
         "[+] Add to cart\n"
         "[-] Remove from cart\n"
         "[=] Calculate cost\n"
         "[O] Checkout\n"
         "[Q] Quit\n");
}

/// @brief Returns true if str is a single character that is a menu key (upper or lower case).
static bool is_menu_choice(char *str)
{
    return strlen(str) == 1 && strchr("AaLlDdEeSsPpCcRr+-=OoQq", str[0]) != NULL;
}

/// @brief Shows the menu until the user enters a valid choice, and returns it in upper case.
static char ask_question_menu(void)
{
    char buf[255];

    do
    {
        print_menu();
        read_string(buf, 255);
    } while (!is_menu_choice(buf));

    return toupper(buf[0]);
}

/// @brief Asks for confirmation (Y) and if given, frees everything in the store.
///        Returns true if the program should quit, false if the user chose to stay.
static bool front_quit(ioopm_webstore_t *store) {

    char *confirmation = ask_question_string("\nAre you sure that you want to quit the program?\n");
    if (strchr("yY", confirmation[0]) == NULL)
        {
            printf("\nYou did not quit the program.\n\n");
            return false;
        }
    
    free(confirmation);
    ioopm_merch_destroy_all(store);
    ioopm_cart_destroy_all(store);
    ioopm_webstore_destroy(store);

    printf("\nYou have quit the program.\n\n");
    return true;
}


/// @brief Main loop: shows the menu, runs the chosen action, and repeats until the user quits.
static void event_loop(ioopm_webstore_t *store)
{
    bool running = true;
    while (running)
    {
        char choice = ask_question_menu();

        switch (choice)
        {
        case 'A':
            ioopm_front_merch_add(store);
            break;

        case 'L':
            ioopm_front_merch_list(store);
            break;

        case 'D':
            ioopm_front_merch_remove(store);
            break;

        case 'E':
            ioopm_front_merch_edit(store);
            break;

        case 'S':
            ioopm_front_merch_show_stock(store);
            break;

        case 'P':
            ioopm_front_merch_replenish(store);
            break;

        case 'C':
            ioopm_front_cart_add_new(store);
            break;

        case 'R':
            ioopm_front_cart_remove(store);
            break;

        case '+':
            ioopm_front_cart_add_item(store);
            break;

        case '-':
            ioopm_front_cart_remove_item(store);
            break;

        case '=':
            ioopm_front_cart_calculate_cost(store);
            break;

        case 'O':
            ioopm_front_cart_checkout(store);
            break;

        case 'Q':
            if (front_quit(store))
            {
                running = false;
            }
            break;
        }
    }
}

/// @brief Entry point: creates an empty webstore and runs the menu.
/// @details All user input is read from stdin. Takes no arguments.
int main(int argc, char *argv[])
{
    if (argc != 1) 
    {
        printf("Usage: %s\n", argv[0]);
    }
    ioopm_webstore_t new_store = ioopm_webstore_create(string_hash, string_eq, int_hash, int_eq);
    event_loop(&new_store);
    return 0;
}