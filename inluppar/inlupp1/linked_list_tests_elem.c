#include <CUnit/Basic.h>
#include <string.h>
#include "linked_list.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

// --- create / destroy ---

void test_create_destroy()
{
  ioopm_list_t *list = ioopm_list_create();
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));
  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
  ioopm_list_destroy(list);
}

// --- append ---

void test_append_once()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 42);

  CU_ASSERT_FALSE(ioopm_list_is_empty(list));
  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 42);

  ioopm_list_destroy(list);
}

void test_append_multiple_preserves_order()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);
  ioopm_list_append(list, 3);

  CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 2).i, 3);

  ioopm_list_destroy(list);
}

// --- prepend ---

void test_prepend_once()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_prepend(list, 42);

  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 42);

  ioopm_list_destroy(list);
}

void test_prepend_multiple_reverses_order()
{
  ioopm_list_t *list = ioopm_list_create();

  // Prepend i ordning 1, 2, 3 -> listan ska bli 3, 2, 1
  ioopm_list_prepend(list, 1);
  ioopm_list_prepend(list, 2);
  ioopm_list_prepend(list, 3);

  CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 3);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 2).i, 1);

  ioopm_list_destroy(list);
}

void test_append_and_prepend_combined()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 20);   // mitten
  ioopm_list_prepend(list, 10);  // fram
  ioopm_list_append(list, 30);   // bak

  // Förväntad ordning: 10, 20, 30
  CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 10);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 20);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 2).i, 30);

  ioopm_list_destroy(list);
}

// --- head / last ---

void test_head_single_element()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 42);

  CU_ASSERT_EQUAL(ioopm_list_head(list).i, 42);

  ioopm_list_destroy(list);
}

void test_head_multiple_elements()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);

  // head ska alltid vara FÖRSTA elementet, oavsett hur många fler som lagts till
  CU_ASSERT_EQUAL(ioopm_list_head(list).i, 1);

  ioopm_list_destroy(list);
}

void test_last_single_element()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 42);

  CU_ASSERT_EQUAL(ioopm_list_last(list).i, 42);

  ioopm_list_destroy(list);
}

void test_last_multiple_elements()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);
  ioopm_list_append(list, 3);

  // last ska alltid vara SISTA elementet
  CU_ASSERT_EQUAL(ioopm_list_last(list).i, 3);

  ioopm_list_destroy(list);
}

// --- insert (vid specifikt index) ---

void test_insert_at_start()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_insert(list, 0, 2);

  CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 1);

  ioopm_list_destroy(list);
}

void test_insert_in_middle()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 3);
  ioopm_list_insert(list, 1, 2); // ska hamna MELLAN 1 och 3

  CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 2).i, 3);

  ioopm_list_destroy(list);
}

void test_insert_at_end()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_insert(list, 1, 2); // index == size, dvs. lägg sist (enligt precondition 0 <= index <= length)

  CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 2);

  ioopm_list_destroy(list);
}

// --- remove ---

void test_remove_only_element()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 42);
  elem_t removed = ioopm_list_remove(list, 0);

  CU_ASSERT_EQUAL(removed.i, 42);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));

  ioopm_list_destroy(list);
}

void test_remove_from_middle()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);
  ioopm_list_append(list, 3);

  elem_t removed = ioopm_list_remove(list, 1); // ta bort mittenelementet (2)

  CU_ASSERT_EQUAL(removed.i, 2);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 1).i, 3);

  ioopm_list_destroy(list);
}

void test_remove_first_element()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);

  elem_t removed = ioopm_list_remove(list, 0);

  CU_ASSERT_EQUAL(removed.i, 1);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 2);

  ioopm_list_destroy(list);
}

void test_remove_last_element()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);

  elem_t removed = ioopm_list_remove(list, 1);

  CU_ASSERT_EQUAL(removed.i, 2);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  CU_ASSERT_EQUAL(ioopm_list_get(list, 0).i, 1);

  ioopm_list_destroy(list);
}

// --- size / is_empty ---

void test_size_empty_list()
{
  ioopm_list_t *list = ioopm_list_create();
  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
  ioopm_list_destroy(list);
}

void test_size_after_several_appends()
{
  ioopm_list_t *list = ioopm_list_create();

  for (int i = 0; i < 5; ++i)
  {
    ioopm_list_append(list, i);
  }

  CU_ASSERT_EQUAL(ioopm_list_size(list), 5);

  ioopm_list_destroy(list);
}

void test_is_empty_true_on_new_list()
{
  ioopm_list_t *list = ioopm_list_create();
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));
  ioopm_list_destroy(list);
}

void test_is_empty_false_after_append()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 1);
  CU_ASSERT_FALSE(ioopm_list_is_empty(list));
  ioopm_list_destroy(list);
}

void test_is_empty_true_after_removing_only_element()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 1);
  ioopm_list_remove(list, 0);
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));
  ioopm_list_destroy(list);
}

int main()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite = CU_add_suite("Linked list tests", init_suite, clean_suite);
  if (my_test_suite == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
      CU_add_test(my_test_suite, "create & destroy", test_create_destroy) == NULL ||
      CU_add_test(my_test_suite, "append once", test_append_once) == NULL ||
      CU_add_test(my_test_suite, "append multiple preserves order", test_append_multiple_preserves_order) == NULL ||
      CU_add_test(my_test_suite, "prepend once", test_prepend_once) == NULL ||
      CU_add_test(my_test_suite, "prepend multiple reverses order", test_prepend_multiple_reverses_order) == NULL ||
      CU_add_test(my_test_suite, "append and prepend combined", test_append_and_prepend_combined) == NULL ||
      CU_add_test(my_test_suite, "head single element", test_head_single_element) == NULL ||
      CU_add_test(my_test_suite, "head multiple elements", test_head_multiple_elements) == NULL ||
      CU_add_test(my_test_suite, "last single element", test_last_single_element) == NULL ||
      CU_add_test(my_test_suite, "last multiple elements", test_last_multiple_elements) == NULL ||
      CU_add_test(my_test_suite, "insert at start", test_insert_at_start) == NULL ||
      CU_add_test(my_test_suite, "insert in middle", test_insert_in_middle) == NULL ||
      CU_add_test(my_test_suite, "insert at end", test_insert_at_end) == NULL ||
      CU_add_test(my_test_suite, "remove only element", test_remove_only_element) == NULL ||
      CU_add_test(my_test_suite, "remove from middle", test_remove_from_middle) == NULL ||
      CU_add_test(my_test_suite, "remove first element", test_remove_first_element) == NULL ||
      CU_add_test(my_test_suite, "remove last element", test_remove_last_element) == NULL ||
      CU_add_test(my_test_suite, "size of empty list", test_size_empty_list) == NULL ||
      CU_add_test(my_test_suite, "size after several appends", test_size_after_several_appends) == NULL ||
      CU_add_test(my_test_suite, "is_empty true on new list", test_is_empty_true_on_new_list) == NULL ||
      CU_add_test(my_test_suite, "is_empty false after append", test_is_empty_false_after_append) == NULL ||
      CU_add_test(my_test_suite, "is_empty true after removing only element", test_is_empty_true_after_removing_only_element) == NULL ||
      0)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}