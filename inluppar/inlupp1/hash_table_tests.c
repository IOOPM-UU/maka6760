#include <CUnit/Basic.h>
#include "/home/lillmacke/IOPM/inluppar/inlupp1/hash_table.h"
#include "hash_table_iterator.h"

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

// These are example test functions. You should replace them with
// functions of your own.
void test_create_destroy()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	CU_ASSERT_PTR_NOT_NULL(ht);
	ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
	// create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	char *key = "abc";
	int value = 123;

	// check that the key is not in ht
	int result = 0;
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, 0);

	// insert key-value pair and check that the mapping exists
	ioopm_hash_table_insert(ht, key, value);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, value);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_existing_key()
{
	// create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	char *key = "abc";
	int old_value = 123;
	int new_value = 456;
	int result = 0;

	// Prep: but "abc" as key in table
	ioopm_hash_table_insert(ht, key, old_value);

	// Confirm key exists and has old_value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, old_value);

	// new_value on same key
	ioopm_hash_table_insert(ht, key, new_value);

	// lookup should give new_value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, new_value);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_two_new_keys()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	char *key1 = "abc";
	char *key2 = "def";
	int result = 0;

	// First insert: key "abc" is new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
	ioopm_hash_table_insert(ht, key1, 1);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 1);

	// Second insert: key "def" is also new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));
	ioopm_hash_table_insert(ht, key2, 2);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
	CU_ASSERT_EQUAL(result, 2);

	// destroy hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_same_key_update_value()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key1 = "abc";
	int result = 0;

	// Case 1: "abc" is new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
	ioopm_hash_table_insert(ht, key1, 1);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 1);

	// Case 2: "abc" already exists, insert should update value
	ioopm_hash_table_insert(ht, key1, 2);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 2); // old value 1 should be gone

	// Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_same_key_then_new_key()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key1 = "abc";
	char *key2 = "def";
	int result = 0;

	// Prep: "abc" already exists
	ioopm_hash_table_insert(ht, key1, 1);

	// Case 2: "abc" is busy, update value
	ioopm_hash_table_insert(ht, key1, 42);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 42);

	// Case 1: "def" is new key
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));
	ioopm_hash_table_insert(ht, key2, 7);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
	CU_ASSERT_EQUAL(result, 7);

	//"abc" should still have its updated value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 42);

	// Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_existing_key_twice()
{
	// Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key = "abc";
	int result = 0;

	// Prep: "abc" already exists
	ioopm_hash_table_insert(ht, key, 1);

	// First update
	ioopm_hash_table_insert(ht, key, 2);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, 2);

	// Second update
	ioopm_hash_table_insert(ht, key, 3);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, 3);

	// destroy the hash table
	ioopm_hash_table_destroy(ht);
}

// All 4 remove tests are made with AI.
void test_remove_existing_single()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	int result = 0;

	ioopm_hash_table_insert(ht, "abc", 123);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));

	int removed_value = ioopm_hash_table_remove(ht, "abc");
	CU_ASSERT_EQUAL(removed_value, 123);

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "abc", &result));

	ioopm_hash_table_destroy(ht);
}

void test_remove_middle_of_list()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	int result = 0;

	ioopm_hash_table_insert(ht, "abc", 1);
	ioopm_hash_table_insert(ht, "def", 2);
	ioopm_hash_table_insert(ht, "ghi", 3);

	int removed_value = ioopm_hash_table_remove(ht, "def");
	CU_ASSERT_EQUAL(removed_value, 2);

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "def", &result));

	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
	CU_ASSERT_EQUAL(result, 1);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "ghi", &result));
	CU_ASSERT_EQUAL(result, 3);

	ioopm_hash_table_destroy(ht);
}

void test_remove_nonexistent_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	int result = 0;

	ioopm_hash_table_remove(ht, "abc");

	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "abc", &result));

	ioopm_hash_table_destroy(ht);
}

void test_remove_nonexistent_nonempty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	int result = 0;

	ioopm_hash_table_insert(ht, "abc", 1);

	ioopm_hash_table_remove(ht, "xyz");

	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
	CU_ASSERT_EQUAL(result, 1);

	ioopm_hash_table_destroy(ht);
}

// Every test from this line on is AI-generated

// 1. Tom tabell, nyckeln finns inte
void test_has_key_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, "k"));

	ioopm_hash_table_destroy(ht);
}

// 2. En nyckel insatt: har den insatta, saknar en annan
void test_has_key_single_insert()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k", 1);

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k"));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, "k2"));

	ioopm_hash_table_destroy(ht);
}

// 3. Tre nycklar insatta: alla tre finns, en fjärde saknas
void test_has_key_multiple_inserts()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k1", 1);
	ioopm_hash_table_insert(ht, "k2", 2);
	ioopm_hash_table_insert(ht, "k3", 3);

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k1"));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k2"));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k3"));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, "k4"));

	ioopm_hash_table_destroy(ht);
}

// 4. Nyckel insatt och direkt borttagen: ska inte finnas längre
void test_has_key_after_insert_and_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k", 1);
	ioopm_hash_table_remove(ht, "k");

	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, "k"));

	ioopm_hash_table_destroy(ht);
}

// 5. Tre nycklar insatta, en borttagen: de kvarvarande finns, den borttagna finns inte
void test_has_key_after_partial_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k1", 1);
	ioopm_hash_table_insert(ht, "k2", 2);
	ioopm_hash_table_insert(ht, "k3", 3);

	ioopm_hash_table_remove(ht, "k2");

	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k1"));
	CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, "k3"));
	CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, "k2"));

	ioopm_hash_table_destroy(ht);
}

// Storlek på en tom tabell
void test_size_empty_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

	ioopm_hash_table_destroy(ht);
}

// Storlek på en tabell med precis en mappning
void test_size_singleton_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k", 1);

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

	ioopm_hash_table_destroy(ht);
}

// Storlek på en tabell med flera mappningar
void test_size_larger_table()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k1", 1);
	ioopm_hash_table_insert(ht, "k2", 2);
	ioopm_hash_table_insert(ht, "k3", 3);
	ioopm_hash_table_insert(ht, "k4", 4);

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 4);

	ioopm_hash_table_destroy(ht);
}

// Storlek efter att ett element tagits bort
void test_size_after_remove()
{
	ioopm_hash_table_t *ht = ioopm_hash_table_create();

	ioopm_hash_table_insert(ht, "k1", 1);
	ioopm_hash_table_insert(ht, "k2", 2);
	ioopm_hash_table_insert(ht, "k3", 3);

	ioopm_hash_table_remove(ht, "k2");

	CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

	ioopm_hash_table_destroy(ht);
}

void test_size_after_removing_more_than_exists()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, "k1", 1);
  ioopm_hash_table_insert(ht, "k2", 2);

  // ta bort båda nycklarna som faktiskt finns
  ioopm_hash_table_remove(ht, "k1");
  ioopm_hash_table_remove(ht, "k2");

  // försök ta bort fler nycklar som ALDRIG funnits i tabellen
  ioopm_hash_table_remove(ht, "k3");
  ioopm_hash_table_remove(ht, "k4");

  // storleken ska vara 0, inte negativ eller felaktig på något sätt
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

// 1. Iterera över en TOM hashtabell
void test_iterator_empty_table()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// 2. Iterera över en tabell med EN entry
void test_iterator_single_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, "abc", 42);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  // iteratorn ska INTE vara vid slutet direkt, eftersom det finns en entry
  CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(it));

  // rätt nyckel och värde ska returneras
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), "abc");
  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it), 42);

  // efter att ha avancerat en gång ska iteratorn nu vara vid slutet
  ioopm_hash_table_iterator_advance(it);
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// 3. Iterera över flera entries -- räkna antalet besök (ordning ej garanterad)
void test_iterator_several_entries()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (int i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
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

// 4. Se till att VARJE insatt nyckel-värde-par besöks EXAKT en gång,
//    och att current_value stämmer med rätt nyckel under iterationen
void test_iterator_visits_each_pair_exactly_once()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (int i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  int seen_abc = 0;
  int seen_qwe = 0;
  int seen_asd = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *current_key = ioopm_hash_table_iterator_current_key(it);
    int current_value = ioopm_hash_table_iterator_current_value(it);

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
      // en okänd nyckel dök upp -- något är fel
      CU_FAIL("Iterator visited an unexpected key");
    }

    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);

  // varje nyckel ska ha besökts EXAKT en gång -- inte 0, inte 2+
  CU_ASSERT_EQUAL(seen_abc, 1);
  CU_ASSERT_EQUAL(seen_qwe, 1);
  CU_ASSERT_EQUAL(seen_asd, 1);
}

// 5. Iterera över entries som GARANTERAT hamnar i SAMMA bucket,
//    oavsett vilken hashfunktion som används
void test_iterator_multiple_entries_same_bucket()
{
  // Vi kan inte lita på att specifika nycklar hamnar i samma bucket
  // eftersom det beror på hashfunktionens implementation. Ett sätt att
  // GARANTERA flera entries i samma bucket, oavsett hashfunktion, är att
  // sätta in fler nycklar än det finns buckets (17 st, enligt DODGE-kommentaren
  // i hash_table.c) -- då måste, enligt lådprincipen (pigeonhole principle),
  // minst en bucket innehålla mer än en entry.

  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *keys[20];
  char buf[20][8];
  for (int i = 0; i < 20; ++i)
  {
    snprintf(buf[i], sizeof(buf[i]), "key%d", i);
    keys[i] = buf[i];
    ioopm_hash_table_insert(ht, keys[i], i);
  }

  int seen[20] = {0};
  int iteration_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *current_key = ioopm_hash_table_iterator_current_key(it);

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
    CU_ASSERT_EQUAL(seen[i], 1); // varje nyckel besökt exakt en gång
  }
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
		CU_add_test(my_test_suite, "iterator over empty table", test_iterator_empty_table) == NULL ||
    	CU_add_test(my_test_suite, "iterator over single entry", test_iterator_single_entry) == NULL ||
    	CU_add_test(my_test_suite, "iterator over several entries", test_iterator_several_entries) == NULL ||
    	CU_add_test(my_test_suite, "iterator visits each pair exactly once", test_iterator_visits_each_pair_exactly_once) == NULL ||
    	CU_add_test(my_test_suite, "iterator over multiple entries in same bucket", test_iterator_multiple_entries_same_bucket) == NULL ||
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