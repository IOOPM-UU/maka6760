#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"

int init_suite(void)
{
	// Change this function if you want to do something *before* you
	// run a test suite
	return 0;
}

int clean_suite(void)
{
	// Change this function if you want to do something *after* you
	// run a test suite
	return 0;
}

// Hash and equality functions passed to the hash table

static size_t string_hash(elem_t key)
{
	size_t result = 0;
	for (char *str = key.s; *str != '\0'; str++)
	{
		result = result * 31 + (unsigned char)*str;
	}
	return result;
}

static bool string_eq(elem_t a, elem_t b)
{
	return strcmp(a.s, b.s) == 0;
}

static size_t int_hash(elem_t key)
{
	return key.i;
}

static bool int_eq(elem_t a, elem_t b)
{
	return a.i == b.i;
}

// Sends every key to the same bucket, so key_eq_fn alone must tell them apart
static size_t constant_hash(elem_t key)
{
	(void)key;
	return 0;
}

// Removes key and discards the value, returns whether the key existed
static bool remove_key(ioopm_hash_table_t *ht, char *key)
{
	elem_t ignored;
	return ioopm_hash_table_remove(ht, string_elem(key), &ignored);
}

void test_create_destroy()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	CU_ASSERT_PTR_NOT_NULL(ht);
	ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
	// create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	char *key = "abc";
	int value = 123;

	// check that the key is not in ht
	elem_t result = int_elem(0);
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, 0);

	// insert key-value pair and check that the mapping exists
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(value));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, value);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}


void test_insert_existing_key()
{
	// create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	char *key = "abc";
	int old_value = 123;
	int new_value = 456;
	elem_t result = int_elem(0);

	// Prep: but "abc" as key in table
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(old_value));

	// Confirm key exists and has old_value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, old_value);

	// new_value on same key
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(new_value));

	// lookup should give new_value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, new_value);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_two_new_keys()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	char *key1 = "abc";
	char *key2 = "def";
	elem_t result = int_elem(0);

	// First insert: key "abc" is new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	ioopm_hash_table_insert(ht, string_elem(key1), int_elem(1));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	CU_ASSERT_EQUAL(result.i, 1);

	// Second insert: key "def" is also new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem(key2), &result));
	ioopm_hash_table_insert(ht, string_elem(key2), int_elem(2));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key2), &result));
	CU_ASSERT_EQUAL(result.i, 2);

	// destroy hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_same_key_update_value()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	char *key1 = "abc";
	elem_t result = int_elem(0);

	// Case 1: "abc" is new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	ioopm_hash_table_insert(ht, string_elem(key1), int_elem(1));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	CU_ASSERT_EQUAL(result.i, 1);

	// Case 2: "abc" already exists, insert should update value
	ioopm_hash_table_insert(ht, string_elem(key1), int_elem(2));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	CU_ASSERT_EQUAL(result.i, 2); // old value 1 should be gone

	// Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_same_key_then_new_key()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	char *key1 = "abc";
	char *key2 = "def";
	elem_t result = int_elem(0);

	// Prep: "abc" already exists
	ioopm_hash_table_insert(ht, string_elem(key1), int_elem(1));

	// Case 2: "abc" is busy, update value
	ioopm_hash_table_insert(ht, string_elem(key1), int_elem(42));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	CU_ASSERT_EQUAL(result.i, 42);

	// Case 1: "def" is new key
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem(key2), &result));
	ioopm_hash_table_insert(ht, string_elem(key2), int_elem(7));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key2), &result));
	CU_ASSERT_EQUAL(result.i, 7);

	//"abc" should still have its updated value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
	CU_ASSERT_EQUAL(result.i, 42);

	// Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_existing_key_twice()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	char *key = "abc";
	elem_t result = int_elem(0);

	// Prep: "abc" already exists
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(1));

	// First update
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(2));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, 2);

	// Second update
	ioopm_hash_table_insert(ht, string_elem(key), int_elem(3));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key), &result));
	CU_ASSERT_EQUAL(result.i, 3);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_remove_existing_single()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	elem_t result = int_elem(0);

	ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));

	elem_t removed_value = int_elem(0);
	CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("abc"), &removed_value));
	CU_ASSERT_EQUAL(removed_value.i, 123);

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));

	ioopm_hash_table_destroy(ht);
}

void test_remove_middle_of_list()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	elem_t result = int_elem(0);

	ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
	ioopm_hash_table_insert(ht, string_elem("def"), int_elem(2));
	ioopm_hash_table_insert(ht, string_elem("ghi"), int_elem(3));

	elem_t removed_value = int_elem(0);
	CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("def"), &removed_value));
	CU_ASSERT_EQUAL(removed_value.i, 2);

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("def"), &result));

	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));
	CU_ASSERT_EQUAL(result.i, 1);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("ghi"), &result));
	CU_ASSERT_EQUAL(result.i, 3);

	ioopm_hash_table_destroy(ht);
}

void test_remove_nonexistent_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	elem_t result = int_elem(0);

	CU_ASSERT_FALSE(remove_key(ht, "abc"));

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));

	ioopm_hash_table_destroy(ht);
}

void test_remove_nonexistent_nonempty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
	elem_t result = int_elem(0);

	ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));

	CU_ASSERT_FALSE(remove_key(ht, "xyz"));

	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));
	CU_ASSERT_EQUAL(result.i, 1);

	ioopm_hash_table_destroy(ht);
}

// Every test from this line on is AI-generated

// 1. Empty table, key does not exist
void test_has_key_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("k")));

	ioopm_hash_table_destroy(ht);
}

// 2. One key inserted: has that key, lacks another
void test_has_key_single_insert()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k"), int_elem(1));

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k")));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("k2")));

	ioopm_hash_table_destroy(ht);
}

// 3. Three keys inserted: all three exist, a fourth does not
void test_has_key_multiple_inserts()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k1"), int_elem(1));
	ioopm_hash_table_insert(ht, string_elem("k2"), int_elem(2));
	ioopm_hash_table_insert(ht, string_elem("k3"), int_elem(3));

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k1")));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k2")));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k3")));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("k4")));

	ioopm_hash_table_destroy(ht);
}

// 4. Key inserted and immediately removed: should no longer exist
void test_has_key_after_insert_and_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k"), int_elem(1));
	CU_ASSERT_TRUE(remove_key(ht, "k"));

	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("k")));

	ioopm_hash_table_destroy(ht);
}

// 5. Three keys inserted, one removed: the remaining exist, the removed one does not
void test_has_key_after_partial_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k1"), int_elem(1));
	ioopm_hash_table_insert(ht, string_elem("k2"), int_elem(2));
	ioopm_hash_table_insert(ht, string_elem("k3"), int_elem(3));

	CU_ASSERT_TRUE(remove_key(ht, "k2"));

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k1")));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("k3")));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("k2")));

	ioopm_hash_table_destroy(ht);
}

// Size of an empty table
void test_size_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

	ioopm_hash_table_destroy(ht);
}

// Size of a table with exactly one mapping
void test_size_singleton_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k"), int_elem(1));

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

	ioopm_hash_table_destroy(ht);
}

// Size of a table with several mappings
void test_size_larger_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k1"), int_elem(1));
	ioopm_hash_table_insert(ht, string_elem("k2"), int_elem(2));
	ioopm_hash_table_insert(ht, string_elem("k3"), int_elem(3));
	ioopm_hash_table_insert(ht, string_elem("k4"), int_elem(4));

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 4);

	ioopm_hash_table_destroy(ht);
}

// Size after an element has been removed
void test_size_after_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

	ioopm_hash_table_insert(ht, string_elem("k1"), int_elem(1));
	ioopm_hash_table_insert(ht, string_elem("k2"), int_elem(2));
	ioopm_hash_table_insert(ht, string_elem("k3"), int_elem(3));

	CU_ASSERT_TRUE(remove_key(ht, "k2"));

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

	ioopm_hash_table_destroy(ht);
}

void test_size_after_removing_more_than_exists()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("k1"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("k2"), int_elem(2));

  // remove both keys that actually exist
  CU_ASSERT_TRUE(remove_key(ht, "k1"));
  CU_ASSERT_TRUE(remove_key(ht, "k2"));

  // try to remove more keys that have NEVER been in the table
  CU_ASSERT_FALSE(remove_key(ht, "k3"));
  CU_ASSERT_FALSE(remove_key(ht, "k4"));

  // size should be 0, not negative or wrong in any way
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_is_empty_on_non_empty_table()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("k"), int_elem(1));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  // and back to empty once the only key is removed
  CU_ASSERT_TRUE(remove_key(ht, "k"));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

// 1. Iterate over an EMPTY hash table
void test_iterator_empty_table()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// 2. Iterate over a table with ONE entry
void test_iterator_single_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(42));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  // the iterator should NOT be at the end right away, since there is an entry
  CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(it));

  // the correct key and value should be returned
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it).s, "abc");
  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, 42);

  // after advancing once the iterator should be at the end
  ioopm_hash_table_iterator_advance(it);
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// 3. Iterate over several entries -- count the visits (order not guaranteed)
void test_iterator_several_entries()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (int i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(values[i]));
  }

  int iteration_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(iteration_count, 3);
}

// 4. Make sure EVERY inserted key-value pair is visited EXACTLY once,
//    and that current_value matches the correct key during iteration
void test_iterator_visits_each_pair_exactly_once()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (int i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(values[i]));
  }

  int seen_abc = 0;
  int seen_qwe = 0;
  int seen_asd = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *current_key = ioopm_hash_table_iterator_current_key(it).s;
    int current_value = ioopm_hash_table_iterator_current_value(it).i;

    if (strcmp(current_key, "abc") == 0)
    {
      seen_abc++;
      CU_ASSERT_EQUAL(current_value, 0);
    }
    else if (strcmp(current_key, "qwe") == 0)
    {
      seen_qwe++;
      CU_ASSERT_EQUAL(current_value, 1);
    }
    else if (strcmp(current_key, "asd") == 0)
    {
      seen_asd++;
      CU_ASSERT_EQUAL(current_value, 2);
    }
    else
    {
      // an unknown key showed up -- something is wrong
      CU_FAIL("Iterator visited an unexpected key");
    }

    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);

  // each key should have been visited EXACTLY once -- not 0, not 2+
  CU_ASSERT_EQUAL(seen_abc, 1);
  CU_ASSERT_EQUAL(seen_qwe, 1);
  CU_ASSERT_EQUAL(seen_asd, 1);
}

// 5. Iterate over entries that are GUARANTEED to end up in the SAME bucket,
//    regardless of which hash function is used
void test_iterator_multiple_entries_same_bucket()
{
  // We cannot rely on specific keys ending up in the same bucket, since
  // that depends on the hash function. One way to GUARANTEE several entries
  // in the same bucket, regardless of hash function, is to insert more keys
  // than there are buckets (17, see No_Buckets in hash_table.c) -- then, by
  // the pigeonhole principle, at least one bucket must hold more than one entry.

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  char *keys[20];
  char buf[20][8];
  for (int i = 0; i < 20; ++i)
  {
    snprintf(buf[i], sizeof(buf[i]), "key%d", i);
    keys[i] = buf[i];
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(i));
  }

  int seen[20] = {0};
  int iteration_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *current_key = ioopm_hash_table_iterator_current_key(it).s;

    for (int i = 0; i < 20; ++i)
    {
      if (strcmp(current_key, keys[i]) == 0)
      {
        seen[i]++;
        break;
      }
    }

    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(iteration_count, 20);
  for (int i = 0; i < 20; ++i)
  {
    CU_ASSERT_EQUAL(seen[i], 1); // each key visited exactly once
  }
}

// ============================================================================
// REMOVE (bool + result)
// ============================================================================

void test_remove_missing_key_leaves_result_untouched()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));

  elem_t result = int_elem(99);
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, string_elem("xyz"), &result));
  CU_ASSERT_EQUAL(result.i, 99);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht);
}

void test_remove_same_key_twice()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(5));

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 5);
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, string_elem("abc"), &result)); // already removed
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_remove_minus_one_value()
{
  // remove used to return -1 as an error value, now -1 should be an ordinary value
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("neg"), int_elem(-1));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("neg"), &result));
  CU_ASSERT_EQUAL(result.i, -1);
  result = int_elem(0);
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("neg"), &result));
  CU_ASSERT_EQUAL(result.i, -1);

  ioopm_hash_table_destroy(ht);
}

void test_lookup_missing_key_leaves_result_untouched()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(42);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("nope"), &result));
  CU_ASSERT_EQUAL(result.i, 42);

  ioopm_hash_table_destroy(ht);
}


// ============================================================================
// ELEM_T VALUES
// ============================================================================

void test_elem_int_extremes()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("min"), int_elem(INT_MIN));
  ioopm_hash_table_insert(ht, string_elem("max"), int_elem(INT_MAX));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("min"), &result));
  CU_ASSERT_EQUAL(result.i, INT_MIN);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("max"), &result));
  CU_ASSERT_EQUAL(result.i, INT_MAX);

  ioopm_hash_table_destroy(ht);
}

void test_elem_unsigned_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("u"), unsigned_elem(UINT_MAX));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("u"), &result));
  CU_ASSERT_EQUAL(result.u, UINT_MAX);

  ioopm_hash_table_destroy(ht);
}

void test_elem_bool_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("yes"), bool_elem(true));
  ioopm_hash_table_insert(ht, string_elem("no"), bool_elem(false));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("yes"), &result));
  CU_ASSERT_TRUE(result.b);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("no"), &result));
  CU_ASSERT_FALSE(result.b);

  ioopm_hash_table_destroy(ht);
}

void test_elem_float_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("pi"), float_elem(3.14f));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("pi"), &result));
  CU_ASSERT_DOUBLE_EQUAL(result.f, 3.14f, 0.0);

  ioopm_hash_table_destroy(ht);
}

void test_elem_string_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("greeting"), string_elem("hello"));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("greeting"), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "hello");

  ioopm_hash_table_destroy(ht);
}

void test_elem_pointer_identity_preserved()
{
  int x = 1;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("ptr"), ptr_elem(&x));

  // The table stores the pointer, not a copy of what it points to
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("ptr"), &result));
  CU_ASSERT_PTR_EQUAL(result.p, &x);
  x = 100;
  CU_ASSERT_EQUAL(*(int *)result.p, 100);

  ioopm_hash_table_destroy(ht);
}

void test_elem_null_pointer_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(1);
  ioopm_hash_table_insert(ht, string_elem("null"), ptr_elem(NULL));

  // NULL is a valid value - the key should exist
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("null")));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("null"), &result));
  CU_ASSERT_PTR_NULL(result.p);

  ioopm_hash_table_destroy(ht);
}

void test_elem_update_changes_type_of_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("k"), int_elem(7));
  ioopm_hash_table_insert(ht, string_elem("k"), string_elem("seven"));

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("k"), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "seven");

  ioopm_hash_table_destroy(ht);
}

void test_elem_remove_returns_same_pointer()
{
  char *str = strdup("heap-string");
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("s"), string_elem(str));

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("s"), &result));
  CU_ASSERT_PTR_EQUAL(result.s, str);

  ioopm_hash_table_destroy(ht);
  free(result.s); // the caller still owns the memory
}

void test_elem_destroy_does_not_free_keys_or_values()
{
  // Spec: destroy frees neither keys nor values - they are owned by the caller
  char *key = strdup("key-string");
  char *value = strdup("value-string");
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, string_elem(key), string_elem(value));
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_STRING_EQUAL(key, "key-string");   // valgrind complains if they are already freed
  CU_ASSERT_STRING_EQUAL(value, "value-string");
  free(key);
  free(value);
}

void test_elem_iterator_string_values()
{
  char *keys[3] = {"a", "b", "c"};
  char *values[3] = {"one", "two", "three"};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (int i = 0; i < 3; ++i)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), string_elem(values[i]));
  }

  int seen = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *key = ioopm_hash_table_iterator_current_key(it).s;
    char *value = ioopm_hash_table_iterator_current_value(it).s;
    for (int i = 0; i < 3; ++i)
    {
      if (strcmp(key, keys[i]) == 0)
      {
        CU_ASSERT_PTR_EQUAL(value, values[i]); // the correct value belongs to the correct key
        seen++;
      }
    }
    ioopm_hash_table_iterator_advance(it);
  }
  CU_ASSERT_EQUAL(seen, 3);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// ============================================================================
// GENERIC KEYS (hash_fn + key_eq_fn)
// ============================================================================

void test_key_equal_strings_different_pointers()
{
  // Two different pointers to equal strings should count as the SAME key,
  // since the table compares with key_eq_fn and not bit by bit
  char key1[] = "same";
  char key2[] = "same";
  CU_ASSERT_PTR_NOT_EQUAL(key1, key2);

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem(key1), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem(key2), int_elem(2)); // should update, not add

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(key1), &result));
  CU_ASSERT_EQUAL(result.i, 2);

  ioopm_hash_table_destroy(ht);
}

void test_key_int_insert_and_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, int_elem(1), string_elem("one"));
  ioopm_hash_table_insert(ht, int_elem(2), string_elem("two"));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(1), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "one");
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(2), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "two");
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(3), &result));

  ioopm_hash_table_destroy(ht);
}

void test_key_int_negative_and_zero()
{
  // Negative ints become large size_t values in int_hash - the table should still handle it
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, int_elem(0), int_elem(10));
  ioopm_hash_table_insert(ht, int_elem(-1), int_elem(20));
  ioopm_hash_table_insert(ht, int_elem(INT_MIN), int_elem(30));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(0), &result));
  CU_ASSERT_EQUAL(result.i, 10);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(-1), &result));
  CU_ASSERT_EQUAL(result.i, 20);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(INT_MIN), &result));
  CU_ASSERT_EQUAL(result.i, 30);

  ioopm_hash_table_destroy(ht);
}

void test_key_int_same_bucket()
{
  // 17 buckets: 1, 18 and 35 end up in the same bucket with int_hash
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  ioopm_hash_table_insert(ht, int_elem(18), int_elem(200));
  ioopm_hash_table_insert(ht, int_elem(35), int_elem(300));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, int_elem(18), &result));
  CU_ASSERT_EQUAL(result.i, 200);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(1), &result));
  CU_ASSERT_EQUAL(result.i, 100);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(35), &result));
  CU_ASSERT_EQUAL(result.i, 300);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(18)));

  ioopm_hash_table_destroy(ht);
}

void test_key_all_in_one_bucket_uses_eq_fn()
{
  // With constant_hash everything ends up in the same bucket - only key_eq_fn can tell the keys apart
  ioopm_hash_table_t *ht = ioopm_hash_table_create(constant_hash, string_eq);
  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("b"), int_elem(2));
  ioopm_hash_table_insert(ht, string_elem("c"), int_elem(3));
  ioopm_hash_table_insert(ht, string_elem("b"), int_elem(22)); // update
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
  CU_ASSERT_EQUAL(result.i, 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("b"), &result));
  CU_ASSERT_EQUAL(result.i, 22);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("c"), &result));
  CU_ASSERT_EQUAL(result.i, 3);

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("a"), &result)); // first in the chain
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("c"), &result)); // last in the chain
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("b")));

  ioopm_hash_table_destroy(ht);
}

void test_key_two_tables_different_key_types()
{
  // Each table has its own functions, they must not get mixed up
  ioopm_hash_table_t *by_int = ioopm_hash_table_create(int_hash, int_eq);
  ioopm_hash_table_t *by_str = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(by_int, int_elem(7), string_elem("seven"));
  ioopm_hash_table_insert(by_str, string_elem("seven"), int_elem(7));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(by_int, int_elem(7), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "seven");
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(by_str, string_elem("seven"), &result));
  CU_ASSERT_EQUAL(result.i, 7);

  ioopm_hash_table_destroy(by_int);
  ioopm_hash_table_destroy(by_str);
}

void test_key_iterator_int_keys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  for (int i = 0; i < 5; ++i)
  {
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(i * 10));
  }

  int seen[5] = {0};
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    int key = ioopm_hash_table_iterator_current_key(it).i;
    CU_ASSERT_TRUE(key >= 0 && key < 5);
    if (key >= 0 && key < 5)
    {
      seen[key]++;
      CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, key * 10);
    }
    ioopm_hash_table_iterator_advance(it);
  }
  for (int i = 0; i < 5; ++i)
  {
    CU_ASSERT_EQUAL(seen[i], 1);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

int main()
{
	// First we try to set up CUnit, and exit if we fail
	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	// We then create an empty test suite and specify the name and
	// the init and cleanup functions
	CU_pSuite my_test_suite = CU_add_suite("Hash table tests", init_suite, clean_suite);
	if (my_test_suite == NULL)
	{
		// If the test suite could not be added, tear down CUnit and exit
		CU_cleanup_registry();
		return CU_get_error();
	}

	// This is where we add the test functions to our test suite.
	// For each call to CU_add_test we specify the test suite, the
	// name or description of the test, and the function that runs
	// the test in question. If you want to add another test, just
	// copy a line below and change the information
	if (
		(CU_add_test(my_test_suite, "create & destroy", test_create_destroy) == NULL) ||
		CU_add_test(my_test_suite, "insert once", test_insert_once) == NULL ||
		CU_add_test(my_test_suite, "insert on existing key", test_insert_existing_key) == NULL ||
		CU_add_test(my_test_suite, "insert two new keys", test_insert_two_new_keys) == NULL ||
		CU_add_test(my_test_suite, "insert on same key update value", test_insert_same_key_update_value) == NULL ||
		CU_add_test(my_test_suite, "insert on same key, then on new key", test_insert_same_key_then_new_key) == NULL ||
		CU_add_test(my_test_suite, "insert existing key twice", test_insert_existing_key_twice) == NULL ||
		CU_add_test(my_test_suite, "remove existing single", test_remove_existing_single) == NULL ||
		CU_add_test(my_test_suite, "remove in middle of list", test_remove_middle_of_list) == NULL ||
		CU_add_test(my_test_suite, "remove non-existent entry from empty table", test_remove_nonexistent_empty_table) == NULL ||
		CU_add_test(my_test_suite, "remove non-existent from non-empty table", test_remove_nonexistent_nonempty_table) == NULL ||
		CU_add_test(my_test_suite, "has_key on empty table", test_has_key_empty_table) == NULL ||
		CU_add_test(my_test_suite, "has_key with single insert", test_has_key_single_insert) == NULL ||
		CU_add_test(my_test_suite, "has_key with multiple inserts", test_has_key_multiple_inserts) == NULL ||
		CU_add_test(my_test_suite, "has_key after insert and remove", test_has_key_after_insert_and_remove) == NULL ||
		CU_add_test(my_test_suite, "has_key after partial remove", test_has_key_after_partial_remove) == NULL ||
		CU_add_test(my_test_suite, "size of empty table", test_size_empty_table) == NULL ||
		CU_add_test(my_test_suite, "size of singleton table", test_size_singleton_table) == NULL ||
		CU_add_test(my_test_suite, "size of larger table", test_size_larger_table) == NULL ||
		CU_add_test(my_test_suite, "size after remove", test_size_after_remove) == NULL ||
		CU_add_test(my_test_suite, "size after removing more than exists", test_size_after_removing_more_than_exists) == NULL ||
		CU_add_test(my_test_suite, "is_empty on non-empty table", test_is_empty_on_non_empty_table) == NULL ||
		CU_add_test(my_test_suite, "iterator over empty table", test_iterator_empty_table) == NULL ||
    	CU_add_test(my_test_suite, "iterator over single entry", test_iterator_single_entry) == NULL ||
    	CU_add_test(my_test_suite, "iterator over several entries", test_iterator_several_entries) == NULL ||
    	CU_add_test(my_test_suite, "iterator visits each pair exactly once", test_iterator_visits_each_pair_exactly_once) == NULL ||
    	CU_add_test(my_test_suite, "iterator over multiple entries in same bucket", test_iterator_multiple_entries_same_bucket) == NULL ||
		CU_add_test(my_test_suite, "remove missing key leaves result untouched", test_remove_missing_key_leaves_result_untouched) == NULL ||
		CU_add_test(my_test_suite, "remove same key twice", test_remove_same_key_twice) == NULL ||
		CU_add_test(my_test_suite, "remove works with value -1", test_remove_minus_one_value) == NULL ||
		CU_add_test(my_test_suite, "lookup missing key leaves result untouched", test_lookup_missing_key_leaves_result_untouched) == NULL ||
		CU_add_test(my_test_suite, "elem int extremes", test_elem_int_extremes) == NULL ||
		CU_add_test(my_test_suite, "elem unsigned value", test_elem_unsigned_value) == NULL ||
		CU_add_test(my_test_suite, "elem bool values", test_elem_bool_values) == NULL ||
		CU_add_test(my_test_suite, "elem float value", test_elem_float_value) == NULL ||
		CU_add_test(my_test_suite, "elem string value", test_elem_string_value) == NULL ||
		CU_add_test(my_test_suite, "elem pointer identity preserved", test_elem_pointer_identity_preserved) == NULL ||
		CU_add_test(my_test_suite, "elem NULL pointer is a valid value", test_elem_null_pointer_value) == NULL ||
		CU_add_test(my_test_suite, "elem update changes type of value", test_elem_update_changes_type_of_value) == NULL ||
		CU_add_test(my_test_suite, "elem remove returns same pointer", test_elem_remove_returns_same_pointer) == NULL ||
		CU_add_test(my_test_suite, "elem destroy does not free keys or values", test_elem_destroy_does_not_free_keys_or_values) == NULL ||
		CU_add_test(my_test_suite, "elem iterator string values", test_elem_iterator_string_values) == NULL ||
		CU_add_test(my_test_suite, "key: equal strings, different pointers", test_key_equal_strings_different_pointers) == NULL ||
		CU_add_test(my_test_suite, "key: int insert and lookup", test_key_int_insert_and_lookup) == NULL ||
		CU_add_test(my_test_suite, "key: int negative and zero", test_key_int_negative_and_zero) == NULL ||
		CU_add_test(my_test_suite, "key: int keys in same bucket", test_key_int_same_bucket) == NULL ||
		CU_add_test(my_test_suite, "key: all in one bucket uses eq_fn", test_key_all_in_one_bucket_uses_eq_fn) == NULL ||
		CU_add_test(my_test_suite, "key: two tables with different key types", test_key_two_tables_different_key_types) == NULL ||
		CU_add_test(my_test_suite, "key: iterator over int keys", test_key_iterator_int_keys) == NULL ||
		0)
	{
		// If adding any of the tests fails, we tear down CUnit and exit
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Set the running mode. Use CU_BRM_VERBOSE for maximum output.
	// Use CU_BRM_NORMAL to only print errors and a summary
	CU_basic_set_mode(CU_BRM_VERBOSE);

	// This is where the tests are actually run!
	CU_basic_run_tests();

	// Tear down CUnit before exiting
	CU_cleanup_registry();
	return CU_get_error();
}