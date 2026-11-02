#include "merch_frontend.h"

/**
 * NOTE: Some of the comments for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

// Helper function that asks the user for a name, description and price, and returns the new merch with an empty storage list.
// The price question is repeated until the price is not negative.
static ioopm_merch_t *new_merch_from_user(char *quest_name, char *quest_desc, char *quest_price) 
{
    char *new_name = ask_question_string(quest_name);
    char *new_desc = ask_question_string(quest_desc);
    int new_price = -1;

    while (new_price < 0)
    {
        new_price = ask_question_int(quest_price);
    }

    ioopm_merch_t *new_merch = ioopm_merch_create(new_name, new_desc, new_price);

    return new_merch;
}

// Frontend function for adding a merch
void ioopm_front_merch_add(ioopm_webstore_t *store) 
{
    char *quest_name = "\nEnter the name of the new merch:";
    char *quest_desc = "\nEnter the description of the new merch:";
    char *quest_price = "\nEnter the price of the new merch:";
    
    ioopm_merch_t *new_merch = new_merch_from_user(quest_name, quest_desc, quest_price);
    
    if (ioopm_merch_add(store, new_merch)) 
    {
        printf("\nThe merch %s has been added! \n\n", ioopm_merch_get_name(new_merch));
    }
    else
    {
        printf("\nThe merch already exists! If you want to edit the merch, choose [E].\n\n");
        ioopm_merch_destroy(new_merch);
    }
}

// Frontend function for showing a list of all merch
void ioopm_front_merch_list(ioopm_webstore_t *store) 
{
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    ioopm_hash_table_iterator_t *merch_iterator = ioopm_hash_table_iterator_create(merch_ht);

    int counter = 1;
    int termination = 20;

    printf("\nList of current merch:\n\n");

    while (true)
    {
        for (; counter <= termination; counter += 1)
        {
            if (ioopm_hash_table_iterator_at_end(merch_iterator))
            {
                printf("\nAll merch are listed!\n\n");
                goto done;
            }
            char *current_name = ioopm_hash_table_iterator_current_key(merch_iterator).s;
            printf("%d. %s\n", counter, current_name);
            ioopm_hash_table_iterator_advance(merch_iterator);
        }
        termination += 20;

        char *answer = ask_question_string("\nDo you want the listing of merch to continue?\n");

        if (strchr("nN", answer[0]) != NULL)
        {
            free(answer);
            goto done;
        }

        free(answer);
        printf("\n");
    }
    done:

    ioopm_hash_table_iterator_destroy(merch_iterator);
}

// Frontend function for removing a merch
void ioopm_front_merch_remove(ioopm_webstore_t *store) 
{
    char *merch_to_remove = ask_question_string("\nEnter the name of the merch you want to remove:");

    char question[200];
    snprintf(question, sizeof(question), "Are you sure that you want to remove %s?", merch_to_remove);
    char *confirmation = ask_question_string(question);

    if (strchr("yY", confirmation[0]) == NULL)
        {
            printf("\nThe removal of %s is canceled.\n\n", merch_to_remove);
            free(merch_to_remove);
            free(confirmation);
            return;
        }

    free(confirmation);
    elem_t result;
    bool removal_success = ioopm_merch_remove(store, merch_to_remove, &result);

    if (removal_success && strcmp(ioopm_merch_get_name(result.p), merch_to_remove) == 0)
    {
        printf("\nThe merch %s has been removed!\n\n", merch_to_remove);
        ioopm_merch_destroy(result.p);
    }
    else 
    {
        printf("\nThe merch %s does not exist, nothing has been removed.\n\n", merch_to_remove);
    }

    free(merch_to_remove);
}

// Frontend function for editing a merch
void ioopm_front_merch_edit(ioopm_webstore_t *store) 
{ 
    char *merch_to_edit = ask_question_string("\nEnter the name of the merch you want to edit:");

    char *quest_name = "\nEnter the new name of the merch:";
    char *quest_desc = "\nEnter the new description of the merch:";
    char *quest_price = "\nEnter the new price of the merch:";
    
    ioopm_merch_t *new_merch = new_merch_from_user(quest_name, quest_desc, quest_price);

    char question[200];
    snprintf(question, sizeof(question), "\nAre you sure that you want to edit %s?", merch_to_edit);
    char *confirmation = ask_question_string(question);

    if (strchr("yY", confirmation[0]) == NULL)
    {
        printf("\nThe edit of %s is canceled.\n\n", merch_to_edit);
        free(merch_to_edit);
        free(confirmation);
        ioopm_merch_destroy(new_merch);
        return;
    }

    if (ioopm_merch_edit(store, merch_to_edit, new_merch))
    {
        printf("\nThe merch %s has been edited! \n\n", merch_to_edit);
    }
    else
    {
        printf("\nThe change could not be applied, nothing has been edited.\n\n");
        ioopm_merch_destroy(new_merch);
    }

    free(merch_to_edit);
    free(confirmation);
}

// Frontend function for showing the stock of a merch
void ioopm_front_merch_show_stock(ioopm_webstore_t *store) 
{
    char *given_merch = ask_question_string("\nEnter the name of the merch for which you want to see the stock:");
    
    ioopm_all_merch_t *merch_ht = store->merch_ht;
    elem_t lookup_result;
    if (!ioopm_hash_table_lookup(merch_ht, string_elem(given_merch), &lookup_result))
    {
        printf("\nThe merch %s does not exist.\n\n", given_merch);
        free(given_merch);
        return;
    }
    
    ioopm_list_t *storage_list = ioopm_merch_get_storage_list(lookup_result.p);
    if (ioopm_list_is_empty(storage_list))
    {
        printf("\nThe merch %s has no storage locations.\n\n", given_merch);
        free(given_merch);
        return;
    }

    ioopm_list_iterator_t *storage_iterator = ioopm_list_iterator_create(storage_list);

    printf("\nList of the storage locations for %s:\n\n", given_merch);
    while (!ioopm_list_iterator_at_end(storage_iterator))
    {

        ioopm_shelf_t *current_shelf = ioopm_list_iterator_current(storage_iterator).p;
        char *shelf_name = ioopm_merch_get_shelf_name(current_shelf);
        size_t quantity = ioopm_merch_get_shelf_quantity(current_shelf);

        printf("%s: %ld items\n", shelf_name, quantity);

        ioopm_list_iterator_advance(storage_iterator);
    }

    printf("\n");
    
    free(given_merch);
    ioopm_list_iterator_destroy(storage_iterator);
}

// Frontend function for increasing the stock of a merch
void ioopm_front_merch_replenish(ioopm_webstore_t *store) 
{
    char *question_shelf = "\nEnter the ID of the shelf for which you want to increase the stock (one capital letter followed by two digits):";
    char *shelf = ask_question(question_shelf, ioopm_merch_is_shelf, convert_to_string).string_value;
    char *merch = ask_question_string("\nEnter the name of the merch for which you want to increase the stock:");
    int amount = ask_question_int("\nEnter the number of merch you want to add:");

    if (ioopm_merch_replenish(store, merch, shelf, amount))
    {
        printf("\nThe stock for %s has been increased on shelf %s! \n\n", merch, shelf);
    }
    else
    {
        printf("\nUnable to replenish: check that the merch %s exists and that the shelf %s does not belong to another merch.\n\n", merch, shelf);
    }

    free(shelf);
    free(merch);
}