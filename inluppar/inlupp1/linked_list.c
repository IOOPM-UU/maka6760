#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"
#include "list_iterator.h"

struct link
{
    int value;
    link_t *next;
};

struct list
{
    link_t *first;
    link_t *last;
};

struct list_iterator
{
    ioopm_list_t *list;
    link_t *previous;
    link_t *current;
};

static link_t *traverse_to_link(ioopm_list_t *list, int steps)
{
    link_t *link = list->first;
    for (int i = 0; i < steps; i++)
    {
        link = link->next;
    }
    return link;
}

static link_t *create_new_link(int value)
{
    link_t *new_link = calloc(1, sizeof(link_t));
    new_link->value = value;
    return new_link;
}

static void only_one_link(ioopm_list_t *list, link_t *new_link)
{
    list->first = new_link;
    list->last = new_link;
}

static bool valid_index(ioopm_list_t *list, int index)
{
    if (index >= 0 && index < ioopm_list_size(list))
    {
        return true;
    }
    else
    {
        return false;
    }
}

ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *empty = calloc(1, sizeof(ioopm_list_t));
    empty->first = NULL;
    empty->last = NULL;
    return empty;
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    link_t *current = list->first;

    while (current != NULL)
    {
        link_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void ioopm_list_append(ioopm_list_t *list, int value)
{
    link_t *new_link = create_new_link(value);
    new_link->next = NULL;

    if (list->first == NULL)
    {
        only_one_link(list, new_link);
    }
    else
    {
        list->last->next = new_link;
        list->last = new_link;
    }
}

void ioopm_list_prepend(ioopm_list_t *list, int value)
{
    link_t *new_link = create_new_link(value);
    new_link->next = list->first;

    if (list->first == NULL)
    {
        only_one_link(list, new_link);
    }
    else
    {
        list->first = new_link;
    }
}

int ioopm_list_head(ioopm_list_t *list)
{
    return list->first->value;
}

int ioopm_list_last(ioopm_list_t *list)
{
    return list->last->value;
}

bool ioopm_list_insert(ioopm_list_t *list, int index, int value)
{
    if (index < 0 || index > ioopm_list_size(list))
    {
        return false;
    }

    if (index == 0)
    {
        ioopm_list_prepend(list, value);
        return true;
    }

    link_t *before = traverse_to_link(list, index - 1);
    link_t *after = before->next;

    if (after == NULL)
    {
        ioopm_list_append(list, value);
    }
    else
    {
        link_t *new_link = create_new_link(value);
        before->next = new_link;
        new_link->next = after;
    }
    return true;
}

bool ioopm_list_remove(ioopm_list_t *list, int index, int *result)
{
    if (!valid_index(list, index))
    {
        return false;
    }

    link_t *current = list->first;

    if (index == 0 && ioopm_list_size(list) != 1)
    {
        link_t *first = current->next;
        *result = current->value;
        free(current);
        list->first = first;
        return true;
    }
    else if (index == 0 && ioopm_list_size(list) == 1)
    {
        *result = current->value;
        free(current);
        list->first = NULL;
        list->last = NULL;
        return true;
    }
    else
    {
        link_t *previous = traverse_to_link(list, index - 1);
        current = previous->next;

        // remove last element in list
        if (index == (ioopm_list_size(list) - 1))
        {
            *result = current->value;
            free(current);
            list->last = previous;
            previous->next = NULL;
            return true;
        }

        link_t *next = current->next;
        *result = current->value;
        free(current);
        previous->next = next;
        return true;
    }
}

bool ioopm_list_get(ioopm_list_t *list, int index, int *result)
{
    if (!valid_index(list, index))
    {
        return false;
    }

    link_t *current = traverse_to_link(list, index);
    *result = current->value;
    return true;
}

int ioopm_list_size(ioopm_list_t *list)
{
    link_t *current = list->first;
    int counter = 1;
    if (ioopm_list_is_empty(list))
    {
        return 0;
    }

    while (current->next != NULL)
    {
        counter++;
        current = current->next;
    }

    return counter;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    if (list->first == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    ioopm_list_iterator_t *it = malloc(sizeof(ioopm_list_iterator_t));
    it->list = l;
    it->previous = NULL;
    it->current = l->first;
    return it;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    if (iter->current == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    iter->previous = iter->current;
    iter->current = iter->current->next;
}

int ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->value;
}

int ioopm_list_iterator_remove(ioopm_list_iterator_t *iter)
{
    link_t *removed = iter->current;
    int result = removed->value;
    link_t *next = removed->next;

    if (iter->previous == NULL)
    {
        iter->list->first = next;
    }
    else
    {
        iter->previous->next = next;
    }

    if (removed == iter->list->last)
    {
        iter->list->last = iter->previous;
    }

    free(removed);
    iter->current = next;
    return result;
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, int element)
{
    link_t *new_link = create_new_link(element);
    new_link->next = iter->current;

    if (iter->previous == NULL)
    {
        iter->list->first = new_link;
    }
    else
    {
        iter->previous->next = new_link;
    }

    if (iter->current == NULL)
    {
        iter->list->last = new_link;
    }

    iter->previous = new_link;
}
