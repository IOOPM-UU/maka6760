#include <CUnit/Basic.h>
#include "/home/lillmacke/IOPM/utils.h"
#include "/home/lillmacke/IOPM/inluppar/inlupp1/hash_table.h"

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

	//Prep: but "abc" as key in table
	ioopm_hash_table_insert(ht, key, old_value);

	//Confirm key exists and has old_value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
	CU_ASSERT_EQUAL(result, old_value);

	//new_value on same key
	ioopm_hash_table_insert(ht, key, new_value);

	//lookup should give new_value
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
	//Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key1 = "abc";
	int result = 0;

	//Case 1: "abc" is new
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
	ioopm_hash_table_insert(ht, key1, 1);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 1);

	//Case 2: "abc" already exists, insert should update value
	ioopm_hash_table_insert(ht, key1, 2);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 2); //old value 1 should be gone

	//Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_same_key_then_new_key()
{
	//Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key1 = "abc";
	char *key2 = "def";
	int result = 0;

	//Prep: "abc" already exists 
	ioopm_hash_table_insert(ht, key1, 1);

	//Case 2: "abc" is busy, update value
	ioopm_hash_table_insert(ht, key1, 42);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 42);

	//Case 1: "def" is new key
	CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));
	ioopm_hash_table_insert(ht, key2, 7);
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
	CU_ASSERT_EQUAL(result, 7);

	//"abc" should still have its updated value
	CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
	CU_ASSERT_EQUAL(result, 42);

	//Destroy the hash table
	ioopm_hash_table_destroy(ht);
}

void test_insert_existing_key_twice()
{
	//Create new hash table
	ioopm_hash_table_t *ht = ioopm_hash_table_create();
	char *key = "abc";
	int result = 0;

	//Prep: "abc" already exists 
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