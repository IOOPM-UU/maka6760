#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

struct link 
{
    elem_t elem;
    link_t *next;
};
struct list 
{
    link_t *first;
    link_t *last;
};

ioopm_list_t *ioopm_list_create(void)
{
    //TODO: STUB
    return NULL;
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    //TODO: STUB
    (void) list;
}

void ioopm_list_append(ioopm_list_t *list, int value)
{
    //TODO: STUB
    (void) list;
    (void) value;
}

void ioopm_list_prepend(ioopm_list_t *list, int value)
{
    //TODO: Stub
    (void) list;
    (void) value;

}

elem_t ioopm_list_head(ioopm_list_t *list)
{
    // TODO: STUB
    (void) list;
    elem_t dummy = {0};
    return dummy;
}

elem_t ioopm_list_last(ioopm_list_t *list)
{
    //TODO: Stub
    (void) list;
    elem_t dummy = {0};
    return dummy;
}

void ioopm_list_insert(ioopm_list_t *list, int index, int value)
{
    (void) list;
    (void) index;
    (void) value;
}

elem_t ioopm_list_remove(ioopm_list_t *list, int index)
{
    // TODO: STUB
    (void) list;
    (void) index;
    elem_t dummy = {0};
    return dummy;
}

elem_t ioopm_list_get(ioopm_list_t *list, int index)
{
    // TODO: STUB
    (void) list;
    (void) index;
    elem_t dummy = {0};
    return dummy;
}

int ioopm_list_size(ioopm_list_t *list)
{
    // TODO: STUB
    (void) list;
    return 0;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    // TODO: STUB
    (void) list;
    return true;
}
