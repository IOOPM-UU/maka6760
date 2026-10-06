#include <CUnit/Basic.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"
#include "list_iterator.h"
#include "common.h"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

static elem_t get_at(ioopm_list_t *list, size_t index)
{
    elem_t result = { 0 };
    CU_ASSERT_TRUE(ioopm_list_get(list, index, &result));
    return result;
}

static elem_t remove_at(ioopm_list_t *list, size_t index)
{
    elem_t result = { 0 };
    CU_ASSERT_TRUE(ioopm_list_remove(list, index, &result));
    return result;
}

//ALL THE TESTS ARE AI-GENERATED USING CLAUDE CODE

// ============================================================================
// 1. CREATE & DESTROY
// ============================================================================

void test_create_not_null(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_PTR_NOT_NULL(list);
    ioopm_list_destroy(list);
}

void test_create_multiple_lists_unique_pointers(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    
    CU_ASSERT_PTR_NOT_NULL(list1);
    CU_ASSERT_PTR_NOT_NULL(list2);
    CU_ASSERT_PTR_NOT_EQUAL(list1, list2);
    
    ioopm_list_destroy(list1);
    ioopm_list_destroy(list2);
}

void test_create_initial_fields_are_null(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_PTR_NOT_NULL(list);
    ioopm_list_destroy(list);
}

void test_destroy_empty_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_destroy(list);
    CU_PASS("Destroy did not crash on an empty list");
}

void test_destroy_multiple_empty_lists(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    ioopm_list_t *list3 = ioopm_list_create();
    
    ioopm_list_destroy(list1);
    ioopm_list_destroy(list2);
    ioopm_list_destroy(list3);
    CU_PASS("Destroying several empty lists in a row works");
}

void test_destroy_and_recreate(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_destroy(list1);

    ioopm_list_t *list2 = ioopm_list_create();
    CU_ASSERT_PTR_NOT_NULL(list2);
    ioopm_list_destroy(list2);
}

void test_destroy_isolated_from_others(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    
    ioopm_list_destroy(list1);
    CU_ASSERT_PTR_NOT_NULL(list2);
    
    ioopm_list_destroy(list2);
}


// ============================================================================
// 2. IS_EMPTY
// ============================================================================

void test_is_empty_on_new_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_is_empty_multiple_new_lists(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    
    CU_ASSERT_TRUE(ioopm_list_is_empty(list1));
    CU_ASSERT_TRUE(ioopm_list_is_empty(list2));
    
    ioopm_list_destroy(list1);
    ioopm_list_destroy(list2);
}

void test_is_empty_after_destroy(void)
{
    ioopm_list_t *list = ioopm_list_create();
    bool empty = ioopm_list_is_empty(list);
    CU_ASSERT_TRUE(empty);
    ioopm_list_destroy(list);
}


// ============================================================================
// 3. SIZE
// ============================================================================

void test_size_on_new_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    ioopm_list_destroy(list);
}

void test_size_multiple_new_lists(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    
    CU_ASSERT_EQUAL(ioopm_list_size(list1), 0);
    CU_ASSERT_EQUAL(ioopm_list_size(list2), 0);
    
    ioopm_list_destroy(list1);
    ioopm_list_destroy(list2);
}

void test_size_does_not_change_on_repeated_calls(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    ioopm_list_destroy(list);
}


// ============================================================================
// 4. APPEND (3 tests - uses size to verify)
// ============================================================================

void test_append_increases_size_by_one(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(42));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}

void test_append_increases_size_multiple(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    ioopm_list_destroy(list);
}

void test_append_changes_is_empty_to_false(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_append(list, int_elem(100));
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}


// ============================================================================
// 5. HEAD (3 tests)
// ============================================================================

void test_head_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(77));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 77);
    ioopm_list_destroy(list);
}

void test_head_multiple_elements_stays_first(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 10); // First inserted should remain head
    ioopm_list_destroy(list);
}

void test_head_negative_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(-99));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, -99);
    ioopm_list_destroy(list);
}


// ============================================================================
// 6. LAST (3 testss)
// ============================================================================

void test_last_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(55));
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 55);
    ioopm_list_destroy(list);
}

void test_last_updates_on_each_append(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 10);
    
    ioopm_list_append(list, int_elem(20));
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 20); //Last should update to new value.
    
    ioopm_list_destroy(list);
}

void test_last_multiple_elements_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 3);
    ioopm_list_destroy(list);
}


// ============================================================================
// 7. PREPEND
// ============================================================================

void test_prepend_increases_size_by_one(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, int_elem(42));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}

void test_prepend_increases_size_multiple(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, int_elem(10));
    ioopm_list_prepend(list, int_elem(20));
    ioopm_list_prepend(list, int_elem(30));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    ioopm_list_destroy(list);
}

void test_prepend_changes_is_empty_to_false(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_prepend(list, int_elem(100));
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_prepend_single_element_is_head(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, int_elem(77));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 77);
    ioopm_list_destroy(list);
}

void test_prepend_multiple_elements_head_is_most_recent(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, int_elem(1));
    ioopm_list_prepend(list, int_elem(2));
    ioopm_list_prepend(list, int_elem(3));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 3); //Last added should be first
    ioopm_list_destroy(list);
}

void test_prepend_does_not_move_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));   // last = 1
    ioopm_list_prepend(list, int_elem(2));  // head = 2, last should still be 1
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 1);
    ioopm_list_destroy(list);
}

void test_prepend_on_empty_list_sets_head_and_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, int_elem(5));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 5);
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 5);
    ioopm_list_destroy(list);
}


// ============================================================================
// 8. INSERT
// ============================================================================

void test_insert_at_start_of_nonempty_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    CU_ASSERT_TRUE(ioopm_list_insert(list, 0, int_elem(2)));
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 2);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 1);
    ioopm_list_destroy(list);
}

void test_insert_in_middle(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(3));
    CU_ASSERT_TRUE(ioopm_list_insert(list, 1, int_elem(2))); // should be between 1 and 3
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    CU_ASSERT_EQUAL(get_at(list, 2).i, 3);
    ioopm_list_destroy(list);
}

void test_insert_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    CU_ASSERT_TRUE(ioopm_list_insert(list, 1, int_elem(2))); // index == length, meaning last
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    ioopm_list_destroy(list);
}

void test_insert_at_end_updates_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    CU_ASSERT_TRUE(ioopm_list_insert(list, 2, int_elem(3)));
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 3);
    ioopm_list_destroy(list);
}


// ============================================================================
// 9. REMOVE
// ============================================================================

void test_remove_only_element_returns_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(42));
    elem_t removed = remove_at(list, 0);
    CU_ASSERT_EQUAL(removed.i, 42);
    ioopm_list_destroy(list);
}

void test_remove_only_element_makes_list_empty(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(42));
    remove_at(list, 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_remove_first_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    elem_t removed = remove_at(list, 0);
    CU_ASSERT_EQUAL(removed.i, 1);
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 2);
    ioopm_list_destroy(list);
}

void test_remove_last_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    elem_t removed = remove_at(list, 1);
    CU_ASSERT_EQUAL(removed.i, 2);
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 1);
    ioopm_list_destroy(list);
}

void test_remove_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    elem_t removed = remove_at(list, 1);
    CU_ASSERT_EQUAL(removed.i, 2);
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 3);
    ioopm_list_destroy(list);
}

void test_remove_decreases_size(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    remove_at(list, 0);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}


// ============================================================================
// 10. GET
// ============================================================================

void test_get_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(42));
    CU_ASSERT_EQUAL(get_at(list, 0).i, 42);
    ioopm_list_destroy(list);
}

void test_get_first_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    ioopm_list_destroy(list);
}

void test_get_last_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    CU_ASSERT_EQUAL(get_at(list, 2).i, 3);
    ioopm_list_destroy(list);
}

void test_get_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    ioopm_list_destroy(list);
}

void test_get_after_prepend_and_append_combined(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(20));   // middle
    ioopm_list_prepend(list, int_elem(10));  // front
    ioopm_list_append(list, int_elem(30));   // back
    // expected order: 10, 20, 30
    CU_ASSERT_EQUAL(get_at(list, 0).i, 10);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 20);
    CU_ASSERT_EQUAL(get_at(list, 2).i, 30);
    ioopm_list_destroy(list);
}


// ============================================================================
// 11. LIST ITERATOR
// ============================================================================

void test_iterator_empty_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(77));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 77);

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_multiple_elements_in_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    //Iterator should visit elements in same order as list
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 1);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 2);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 3);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_visits_each_element_exactly_once(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));
    ioopm_list_append(list, int_elem(40));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    int visited = 0;
    while (!ioopm_list_iterator_at_end(iter))
    {
        visited++;
        ioopm_list_iterator_advance(iter);
    }
    CU_ASSERT_EQUAL(visited, ioopm_list_size(list));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_full_traversal_does_not_modify_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    while (!ioopm_list_iterator_at_end(iter))
    {
        ioopm_list_iterator_advance(iter);
    }

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    CU_ASSERT_EQUAL(get_at(list, 2).i, 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_two_independent_iterators(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_iterator_t *iter1 = ioopm_list_iterator_create(list);
    ioopm_list_iterator_t *iter2 = ioopm_list_iterator_create(list);

    //Moves iter1 forward 
    ioopm_list_iterator_advance(iter1);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter1).i, 2);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter2).i, 1); //should not change

    ioopm_list_iterator_destroy(iter1);
    ioopm_list_iterator_destroy(iter2);
    ioopm_list_destroy(list);
}

void test_iterator_reflects_prepend_and_append_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(20));   // middle
    ioopm_list_prepend(list, int_elem(10));  // front
    ioopm_list_append(list, int_elem(30));   // back
    // Expected list order: 10, 20, 30
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 10);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 20);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 30);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_destroy_before_reaching_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter);
    //Destroys iterator in the middle of traversing
    ioopm_list_iterator_destroy(iter);
    CU_PASS("Destroy didn't crash even though the iterator hadn't reached the end");

    ioopm_list_destroy(list);
}

// --- Iterator remove ---

void test_iterator_remove_returns_current_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    elem_t removed = ioopm_list_iterator_remove(iter);
    CU_ASSERT_EQUAL(removed.i, 1);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_remove_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // on element 2
    elem_t removed = ioopm_list_iterator_remove(iter);

    CU_ASSERT_EQUAL(removed.i, 2);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

// --- Iterator insert ---

void test_iterator_insert_increases_size(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // on element 3
    ioopm_list_iterator_insert(iter, int_elem(2)); // 2 should be before 3

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(get_at(list, 0).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    CU_ASSERT_EQUAL(get_at(list, 2).i, 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_insert_keeps_current_unchanged(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // on element 3
    ioopm_list_iterator_insert(iter, int_elem(2));

    // According to spec: "making the current element its next" - current should still be 3
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_insert_at_front(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(2));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    // iterator has not moved, so there is no previous link - insert must update first
    ioopm_list_iterator_insert(iter, int_elem(1));

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_head(list).i, 1);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 2);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 2);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_insert_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(2));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    // move the iterator past the last element - insert must update last
    ioopm_list_iterator_advance(iter);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));
    ioopm_list_iterator_insert(iter, int_elem(3));

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(list).i, 3);
    CU_ASSERT_EQUAL(get_at(list, 2).i, 3);

    // appending after the insert should end up after the new last element
    ioopm_list_append(list, int_elem(4));
    CU_ASSERT_EQUAL(get_at(list, 3).i, 4);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}


// ============================================================================
// 12. ELEM_T UNION
// ============================================================================

void test_elem_int_extremes(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(INT_MIN));
    ioopm_list_append(list, int_elem(0));
    ioopm_list_append(list, int_elem(INT_MAX));
    CU_ASSERT_EQUAL(get_at(list, 0).i, INT_MIN);
    CU_ASSERT_EQUAL(get_at(list, 1).i, 0);
    CU_ASSERT_EQUAL(get_at(list, 2).i, INT_MAX);
    ioopm_list_destroy(list);
}

void test_elem_unsigned_values(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, unsigned_elem(0));
    ioopm_list_append(list, unsigned_elem(UINT_MAX)); // larger than INT_MAX, can not read as negative
    CU_ASSERT_EQUAL(ioopm_list_head(list).u, 0);
    CU_ASSERT_EQUAL(ioopm_list_last(list).u, UINT_MAX);
    ioopm_list_destroy(list);
}

void test_elem_bool_values(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, bool_elem(true));
    ioopm_list_append(list, bool_elem(false));
    CU_ASSERT_TRUE(get_at(list, 0).b);
    CU_ASSERT_FALSE(get_at(list, 1).b);
    ioopm_list_destroy(list);
}

void test_elem_float_values(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, float_elem(3.14f));
    ioopm_list_append(list, float_elem(-0.5f));
    ioopm_list_append(list, float_elem(1e30f));
    // Same float value in should give the same bit pattern out, so exact comparison is ok here
    CU_ASSERT_DOUBLE_EQUAL(get_at(list, 0).f, 3.14f, 0.0);
    CU_ASSERT_DOUBLE_EQUAL(get_at(list, 1).f, -0.5f, 0.0);
    CU_ASSERT_DOUBLE_EQUAL(get_at(list, 2).f, 1e30f, 0.0);
    ioopm_list_destroy(list);
}

void test_elem_pointer_identity_preserved(void)
{
    int a = 1;
    int b = 2;
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, ptr_elem(&a));
    ioopm_list_append(list, ptr_elem(&b));

    //List should stor the pointer itself, not a copy of what it points at
    CU_ASSERT_PTR_EQUAL(ioopm_list_head(list).p, &a);
    CU_ASSERT_PTR_EQUAL(ioopm_list_last(list).p, &b);

    //Changes via pointer is seen when we print it again
    a = 100;
    CU_ASSERT_EQUAL(*(int *)ioopm_list_head(list).p, 100);
    ioopm_list_destroy(list);
}

void test_elem_null_pointer(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, ptr_elem(NULL));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1); // NULL is valid element, not "empty"
    CU_ASSERT_PTR_NULL(ioopm_list_head(list).p);
    ioopm_list_destroy(list);
}

void test_elem_string_values(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, string_elem("hello"));
    ioopm_list_append(list, string_elem("there"));
    ioopm_list_append(list, string_elem(""));
    CU_ASSERT_STRING_EQUAL(get_at(list, 0).s, "hello");
    CU_ASSERT_STRING_EQUAL(get_at(list, 1).s, "there");
    CU_ASSERT_STRING_EQUAL(get_at(list, 2).s, "");
    ioopm_list_destroy(list);
}

void test_elem_string_is_not_copied(void)
{
    char buf[] = "abc";
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, string_elem(buf));

    CU_ASSERT_PTR_EQUAL(ioopm_list_head(list).s, buf);
    buf[0] = 'x';
    CU_ASSERT_STRING_EQUAL(ioopm_list_head(list).s, "xbc");
    ioopm_list_destroy(list);
}

void test_elem_destroy_does_not_free_elements(void)
{   
    //Spec: destroy doesn't free elements memory, it's owned by the caller
    char *str = strdup("heap-string");
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, string_elem(str));
    ioopm_list_destroy(list);

    CU_ASSERT_STRING_EQUAL(str, "heap-string"); // valgrind complains if its already freed
    free(str);
}

void test_elem_remove_string_returns_same_pointer(void)
{
    char *str = strdup("remove me");
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, string_elem("remaining"));
    ioopm_list_append(list, string_elem(str));

    elem_t removed = remove_at(list, 1);
    CU_ASSERT_PTR_EQUAL(removed.s, str);
    CU_ASSERT_STRING_EQUAL(ioopm_list_last(list).s, "remaining");

    ioopm_list_destroy(list);
    free(removed.s); //caller still owns the memory
}

void test_elem_insert_and_get_strings(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, string_elem("a"));
    ioopm_list_append(list, string_elem("c"));
    CU_ASSERT_TRUE(ioopm_list_insert(list, 1, string_elem("b")));
    CU_ASSERT_STRING_EQUAL(get_at(list, 0).s, "a");
    CU_ASSERT_STRING_EQUAL(get_at(list, 1).s, "b");
    CU_ASSERT_STRING_EQUAL(get_at(list, 2).s, "c");
    ioopm_list_destroy(list);
}

void test_elem_get_invalid_index_leaves_result_untouched(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(1));

    elem_t result = int_elem(-1);
    CU_ASSERT_FALSE(ioopm_list_get(list, 5, &result));
    CU_ASSERT_EQUAL(result.i, -1);

    CU_ASSERT_FALSE(ioopm_list_remove(list, 5, &result));
    CU_ASSERT_EQUAL(result.i, -1);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}

void test_elem_insert_invalid_index_does_nothing(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_FALSE(ioopm_list_insert(list, 1, int_elem(1))); //valid intervall for empty list is [0,0]
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_elem_iterator_over_strings(void)
{
    char *words[] = { "one", "two", "three" };
    ioopm_list_t *list = ioopm_list_create();
    for (int i = 0; i < 3; i++)
    {
        ioopm_list_append(list, string_elem(words[i]));
    }

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    for (int i = 0; i < 3; i++)
    {
        CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
        CU_ASSERT_PTR_EQUAL(ioopm_list_iterator_current(iter).s, words[i]);
        ioopm_list_iterator_advance(iter);
    }
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_elem_iterator_insert_and_remove_pointers(void)
{
    int a = 1, b = 2, c = 3;
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, ptr_elem(&a));
    ioopm_list_append(list, ptr_elem(&c));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter);           // on &c
    ioopm_list_iterator_insert(iter, ptr_elem(&b)); // &b before &c
    CU_ASSERT_PTR_EQUAL(get_at(list, 1).p, &b);

    elem_t removed = ioopm_list_iterator_remove(iter); // removes &c
    CU_ASSERT_PTR_EQUAL(removed.p, &c);
    CU_ASSERT_PTR_EQUAL(ioopm_list_last(list).p, &b);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_elem_multiple_lists_different_types(void)
{
    //Same list implementation should work for different element types at the same time
    ioopm_list_t *ints = ioopm_list_create();
    ioopm_list_t *strs = ioopm_list_create();
    ioopm_list_t *floats = ioopm_list_create();

    ioopm_list_append(ints, int_elem(42));
    ioopm_list_append(strs, string_elem("forty-two"));
    ioopm_list_append(floats, float_elem(4.2f));

    CU_ASSERT_EQUAL(ioopm_list_head(ints).i, 42);
    CU_ASSERT_STRING_EQUAL(ioopm_list_head(strs).s, "forty-two");
    CU_ASSERT_DOUBLE_EQUAL(ioopm_list_head(floats).f, 4.2f, 0.0);

    ioopm_list_destroy(ints);
    ioopm_list_destroy(strs);
    ioopm_list_destroy(floats);
}


// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Linked List Tests - Progressive TDD", init_suite, clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        // 1. Create & Destroy
        (CU_add_test(suite, "Create returns non-null", test_create_not_null) == NULL) ||
        (CU_add_test(suite, "Create returns unique pointers", test_create_multiple_lists_unique_pointers) == NULL) ||
        (CU_add_test(suite, "Create initial state basic check", test_create_initial_fields_are_null) == NULL) ||
        (CU_add_test(suite, "Destroy empty list works", test_destroy_empty_list) == NULL) ||
        (CU_add_test(suite, "Destroy multiple empty lists works", test_destroy_multiple_empty_lists) == NULL) ||
        (CU_add_test(suite, "Destroy and recreate list works", test_destroy_and_recreate) == NULL) ||
        (CU_add_test(suite, "Destroy isolated lists works", test_destroy_isolated_from_others) == NULL) ||

        // 2. Is Empty
        (CU_add_test(suite, "Is empty on new list", test_is_empty_on_new_list) == NULL) ||
        (CU_add_test(suite, "Is empty multiple new lists", test_is_empty_multiple_new_lists) == NULL) ||
        (CU_add_test(suite, "Is empty after check", test_is_empty_after_destroy) == NULL) ||

        // 3. Size
        (CU_add_test(suite, "Size on new list is zero", test_size_on_new_list) == NULL) ||
        (CU_add_test(suite, "Size multiple new lists", test_size_multiple_new_lists) == NULL) ||
        (CU_add_test(suite, "Size persistent on repeated calls", test_size_does_not_change_on_repeated_calls) == NULL) ||

        // 4. Append
        (CU_add_test(suite, "Append increases size by one", test_append_increases_size_by_one) == NULL) ||
        (CU_add_test(suite, "Append increases size multiple", test_append_increases_size_multiple) == NULL) ||
        (CU_add_test(suite, "Append changes is empty to false", test_append_changes_is_empty_to_false) == NULL) ||

        // 5. Head
        (CU_add_test(suite, "Head single element", test_head_single_element) == NULL) ||
        (CU_add_test(suite, "Head multiple elements stays first", test_head_multiple_elements_stays_first) == NULL) ||
        (CU_add_test(suite, "Head negative value", test_head_negative_value) == NULL) ||

        // 6. Last
        (CU_add_test(suite, "Last single element", test_last_single_element) == NULL) ||
        (CU_add_test(suite, "Last updates on each append", test_last_updates_on_each_append) == NULL) ||
        (CU_add_test(suite, "Last multiple elements order", test_last_multiple_elements_order) == NULL) ||

        // 7. Prepend
        (CU_add_test(suite, "Prepend increases size by one", test_prepend_increases_size_by_one) == NULL) ||
        (CU_add_test(suite, "Prepend increases size multiple", test_prepend_increases_size_multiple) == NULL) ||
        (CU_add_test(suite, "Prepend changes is empty to false", test_prepend_changes_is_empty_to_false) == NULL) ||
        (CU_add_test(suite, "Prepend single element is head", test_prepend_single_element_is_head) == NULL) ||
        (CU_add_test(suite, "Prepend multiple elements head is most recent", test_prepend_multiple_elements_head_is_most_recent) == NULL) ||
        (CU_add_test(suite, "Prepend does not move last", test_prepend_does_not_move_last) == NULL) ||
        (CU_add_test(suite, "Prepend on empty list sets head and last", test_prepend_on_empty_list_sets_head_and_last) == NULL) ||

        // 8. Insert
        (CU_add_test(suite, "Insert at start of nonempty list", test_insert_at_start_of_nonempty_list) == NULL) ||
        (CU_add_test(suite, "Insert in middle", test_insert_in_middle) == NULL) ||
        (CU_add_test(suite, "Insert at end", test_insert_at_end) == NULL) ||
        (CU_add_test(suite, "Insert at end updates last", test_insert_at_end_updates_last) == NULL) ||

        // 9. Remove
        (CU_add_test(suite, "Remove only element returns value", test_remove_only_element_returns_value) == NULL) ||
        (CU_add_test(suite, "Remove only element makes list empty", test_remove_only_element_makes_list_empty) == NULL) ||
        (CU_add_test(suite, "Remove first element", test_remove_first_element) == NULL) ||
        (CU_add_test(suite, "Remove last element", test_remove_last_element) == NULL) ||
        (CU_add_test(suite, "Remove middle element", test_remove_middle_element) == NULL) ||
        (CU_add_test(suite, "Remove decreases size", test_remove_decreases_size) == NULL) ||

        // 10. Get
        (CU_add_test(suite, "Get single element", test_get_single_element) == NULL) ||
        (CU_add_test(suite, "Get first element", test_get_first_element) == NULL) ||
        (CU_add_test(suite, "Get last element", test_get_last_element) == NULL) ||
        (CU_add_test(suite, "Get middle element", test_get_middle_element) == NULL) ||
        (CU_add_test(suite, "Get after prepend and append combined", test_get_after_prepend_and_append_combined) == NULL) ||

        // 11. List iterator
        (CU_add_test(suite, "Iterator over empty list", test_iterator_empty_list) == NULL) ||
        (CU_add_test(suite, "Iterator over single element", test_iterator_single_element) == NULL) ||
        (CU_add_test(suite, "Iterator visits multiple elements in order", test_iterator_multiple_elements_in_order) == NULL) ||
        (CU_add_test(suite, "Iterator visits each element exactly once", test_iterator_visits_each_element_exactly_once) == NULL) ||
        (CU_add_test(suite, "Iterator full traversal does not modify list", test_iterator_full_traversal_does_not_modify_list) == NULL) ||
        (CU_add_test(suite, "Two independent iterators over same list", test_iterator_two_independent_iterators) == NULL) ||
        (CU_add_test(suite, "Iterator reflects prepend and append order", test_iterator_reflects_prepend_and_append_order) == NULL) ||
        (CU_add_test(suite, "Iterator destroy before reaching end", test_iterator_destroy_before_reaching_end) == NULL) ||
        (CU_add_test(suite, "Iterator remove returns current value", test_iterator_remove_returns_current_value) == NULL) ||
        (CU_add_test(suite, "Iterator remove middle element", test_iterator_remove_middle_element) == NULL) ||
        (CU_add_test(suite, "Iterator insert increases size", test_iterator_insert_increases_size) == NULL) ||
        (CU_add_test(suite, "Iterator insert keeps current unchanged", test_iterator_insert_keeps_current_unchanged) == NULL) ||
        (CU_add_test(suite, "Iterator insert at front updates first", test_iterator_insert_at_front) == NULL) ||
        (CU_add_test(suite, "Iterator insert at end updates last", test_iterator_insert_at_end) == NULL) ||

        // 12. elem_t union
        (CU_add_test(suite, "Elem int extremes", test_elem_int_extremes) == NULL) ||
        (CU_add_test(suite, "Elem unsigned values", test_elem_unsigned_values) == NULL) ||
        (CU_add_test(suite, "Elem bool values", test_elem_bool_values) == NULL) ||
        (CU_add_test(suite, "Elem float values", test_elem_float_values) == NULL) ||
        (CU_add_test(suite, "Elem pointer identity preserved", test_elem_pointer_identity_preserved) == NULL) ||
        (CU_add_test(suite, "Elem NULL pointer is a valid element", test_elem_null_pointer) == NULL) ||
        (CU_add_test(suite, "Elem string values", test_elem_string_values) == NULL) ||
        (CU_add_test(suite, "Elem string is not copied", test_elem_string_is_not_copied) == NULL) ||
        (CU_add_test(suite, "Elem destroy does not free elements", test_elem_destroy_does_not_free_elements) == NULL) ||
        (CU_add_test(suite, "Elem remove string returns same pointer", test_elem_remove_string_returns_same_pointer) == NULL) ||
        (CU_add_test(suite, "Elem insert and get strings", test_elem_insert_and_get_strings) == NULL) ||
        (CU_add_test(suite, "Elem invalid get/remove leaves result untouched", test_elem_get_invalid_index_leaves_result_untouched) == NULL) ||
        (CU_add_test(suite, "Elem insert invalid index does nothing", test_elem_insert_invalid_index_does_nothing) == NULL) ||
        (CU_add_test(suite, "Elem iterator over strings", test_elem_iterator_over_strings) == NULL) ||
        (CU_add_test(suite, "Elem iterator insert and remove pointers", test_elem_iterator_insert_and_remove_pointers) == NULL) ||
        (CU_add_test(suite, "Elem multiple lists different types", test_elem_multiple_lists_different_types) == NULL)
    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}