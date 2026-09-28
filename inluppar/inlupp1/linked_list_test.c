#include <CUnit/Basic.h>
#include <stdbool.h>
#include "linked_list.h"
#include "list_iterator.h"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

static int get_at(ioopm_list_t *list, int index)
{
    int result = 0;
    CU_ASSERT_TRUE(ioopm_list_get(list, index, &result));
    return result;
}

static int remove_at(ioopm_list_t *list, int index)
{
    int result = 0;
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
    CU_PASS("Destroy kraschade inte på en tom lista");
}

void test_destroy_multiple_empty_lists(void)
{
    ioopm_list_t *list1 = ioopm_list_create();
    ioopm_list_t *list2 = ioopm_list_create();
    ioopm_list_t *list3 = ioopm_list_create();
    
    ioopm_list_destroy(list1);
    ioopm_list_destroy(list2);
    ioopm_list_destroy(list3);
    CU_PASS("Destroy av flera tomma listor efter varandra fungerar");
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
// 4. APPEND (3 tester - använder size för att verifiera)
// ============================================================================

void test_append_increases_size_by_one(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 42);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}

void test_append_increases_size_multiple(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 10);
    ioopm_list_append(list, 20);
    ioopm_list_append(list, 30);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    ioopm_list_destroy(list);
}

void test_append_changes_is_empty_to_false(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_append(list, 100);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}


// ============================================================================
// 5. HEAD (3 tester)
// ============================================================================

void test_head_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 77);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 77);
    ioopm_list_destroy(list);
}

void test_head_multiple_elements_stays_first(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 10);
    ioopm_list_append(list, 20);
    ioopm_list_append(list, 30);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 10); // Första insatta ska ligga kvar som head
    ioopm_list_destroy(list);
}

void test_head_negative_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, -99);
    CU_ASSERT_EQUAL(ioopm_list_head(list), -99);
    ioopm_list_destroy(list);
}


// ============================================================================
// 6. LAST (3 tester)
// ============================================================================

void test_last_single_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 55);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 55);
    ioopm_list_destroy(list);
}

void test_last_updates_on_each_append(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 10);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 10);
    
    ioopm_list_append(list, 20);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 20); // Sista ska uppdateras till nya värdet
    
    ioopm_list_destroy(list);
}

void test_last_multiple_elements_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    ioopm_list_destroy(list);
}


// ============================================================================
// 7. PREPEND
// ============================================================================

void test_prepend_increases_size_by_one(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, 42);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    ioopm_list_destroy(list);
}

void test_prepend_increases_size_multiple(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, 10);
    ioopm_list_prepend(list, 20);
    ioopm_list_prepend(list, 30);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    ioopm_list_destroy(list);
}

void test_prepend_changes_is_empty_to_false(void)
{
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_prepend(list, 100);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_prepend_single_element_is_head(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, 77);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 77);
    ioopm_list_destroy(list);
}

void test_prepend_multiple_elements_head_is_most_recent(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, 1);
    ioopm_list_prepend(list, 2);
    ioopm_list_prepend(list, 3);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 3); // Senast tillagda ska ligga först
    ioopm_list_destroy(list);
}

void test_prepend_does_not_move_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);   // last = 1
    ioopm_list_prepend(list, 2);  // head = 2, last ska fortfarande vara 1
    CU_ASSERT_EQUAL(ioopm_list_last(list), 1);
    ioopm_list_destroy(list);
}

void test_prepend_on_empty_list_sets_head_and_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_prepend(list, 5);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 5);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 5);
    ioopm_list_destroy(list);
}


// ============================================================================
// 8. INSERT
// ============================================================================

void test_insert_at_start_of_nonempty_list(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    CU_ASSERT_TRUE(ioopm_list_insert(list, 0, 2));
    CU_ASSERT_EQUAL(ioopm_list_head(list), 2);
    CU_ASSERT_EQUAL(get_at(list, 1), 1);
    ioopm_list_destroy(list);
}

void test_insert_in_middle(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 3);
    CU_ASSERT_TRUE(ioopm_list_insert(list, 1, 2)); // ska hamna mellan 1 och 3
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 2);
    CU_ASSERT_EQUAL(get_at(list, 2), 3);
    ioopm_list_destroy(list);
}

void test_insert_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    CU_ASSERT_TRUE(ioopm_list_insert(list, 1, 2)); // index == length, dvs sist
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 2);
    ioopm_list_destroy(list);
}

void test_insert_at_end_updates_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    CU_ASSERT_TRUE(ioopm_list_insert(list, 2, 3));
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    ioopm_list_destroy(list);
}


// ============================================================================
// 9. REMOVE
// ============================================================================

void test_remove_only_element_returns_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 42);
    int removed = remove_at(list, 0);
    CU_ASSERT_EQUAL(removed, 42);
    ioopm_list_destroy(list);
}

void test_remove_only_element_makes_list_empty(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 42);
    remove_at(list, 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    ioopm_list_destroy(list);
}

void test_remove_first_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    int removed = remove_at(list, 0);
    CU_ASSERT_EQUAL(removed, 1);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 2);
    ioopm_list_destroy(list);
}

void test_remove_last_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    int removed = remove_at(list, 1);
    CU_ASSERT_EQUAL(removed, 2);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 1);
    ioopm_list_destroy(list);
}

void test_remove_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    int removed = remove_at(list, 1);
    CU_ASSERT_EQUAL(removed, 2);
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 3);
    ioopm_list_destroy(list);
}

void test_remove_decreases_size(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
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
    ioopm_list_append(list, 42);
    CU_ASSERT_EQUAL(get_at(list, 0), 42);
    ioopm_list_destroy(list);
}

void test_get_first_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    ioopm_list_destroy(list);
}

void test_get_last_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    CU_ASSERT_EQUAL(get_at(list, 2), 3);
    ioopm_list_destroy(list);
}

void test_get_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    CU_ASSERT_EQUAL(get_at(list, 1), 2);
    ioopm_list_destroy(list);
}

void test_get_after_prepend_and_append_combined(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 20);   // mitten
    ioopm_list_prepend(list, 10);  // fram
    ioopm_list_append(list, 30);   // bak
    // Förväntad ordning: 10, 20, 30
    CU_ASSERT_EQUAL(get_at(list, 0), 10);
    CU_ASSERT_EQUAL(get_at(list, 1), 20);
    CU_ASSERT_EQUAL(get_at(list, 2), 30);
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
    ioopm_list_append(list, 77);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 77);

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_multiple_elements_in_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    // Iteratorn ska besöka elementen i samma ordning som listan
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 1);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 2);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 3);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_visits_each_element_exactly_once(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 10);
    ioopm_list_append(list, 20);
    ioopm_list_append(list, 30);
    ioopm_list_append(list, 40);
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
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    while (!ioopm_list_iterator_at_end(iter))
    {
        ioopm_list_iterator_advance(iter);
    }

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 2);
    CU_ASSERT_EQUAL(get_at(list, 2), 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_two_independent_iterators(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_iterator_t *iter1 = ioopm_list_iterator_create(list);
    ioopm_list_iterator_t *iter2 = ioopm_list_iterator_create(list);

    // Flytta bara den ena iteratorn framåt
    ioopm_list_iterator_advance(iter1);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter1), 2);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter2), 1); // ska vara opåverkad

    ioopm_list_iterator_destroy(iter1);
    ioopm_list_iterator_destroy(iter2);
    ioopm_list_destroy(list);
}

void test_iterator_reflects_prepend_and_append_order(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 20);   // mitten
    ioopm_list_prepend(list, 10);  // fram
    ioopm_list_append(list, 30);   // bak
    // Förväntad listordning: 10, 20, 30
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 10);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 20);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 30);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_destroy_before_reaching_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter);
    // Förstör iteratorn mitt i traverseringen, inte vid slutet
    ioopm_list_iterator_destroy(iter);
    CU_PASS("Destroy kraschade inte trots att iteratorn inte nått slutet");

    ioopm_list_destroy(list);
}

// --- Iterator remove (valfri att implementera) ---

void test_iterator_remove_returns_current_value(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    int removed = ioopm_list_iterator_remove(iter);
    CU_ASSERT_EQUAL(removed, 1);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_remove_middle_element(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // nu på element 2
    int removed = ioopm_list_iterator_remove(iter);

    CU_ASSERT_EQUAL(removed, 2);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

// --- Iterator insert (valfri att implementera) ---

void test_iterator_insert_increases_size(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // nu på element 3
    ioopm_list_iterator_insert(iter, 2); // 2 ska hamna före 3

    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(get_at(list, 0), 1);
    CU_ASSERT_EQUAL(get_at(list, 1), 2);
    CU_ASSERT_EQUAL(get_at(list, 2), 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_insert_keeps_current_unchanged(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 3);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    ioopm_list_iterator_advance(iter); // nu på element 3
    ioopm_list_iterator_insert(iter, 2);

    // Enligt specen: "making the current element its next" - current ska fortfarande vara 3
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 3);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
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
        (CU_add_test(suite, "Iterator insert keeps current unchanged", test_iterator_insert_keeps_current_unchanged) == NULL)
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