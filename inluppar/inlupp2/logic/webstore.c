#include "webstore.h"
#include "merch.h"
#include "hash_table_iterator.h"


// Creates a webstore with three empty hash tables
ioopm_webstore_t ioopm_webstore_create(ioopm_hash_function *string_hash, ioopm_eq_function *string_eq, ioopm_hash_function *int_hash, ioopm_eq_function *int_eq)
{
	ioopm_webstore_t store = {
		.merch_ht = ioopm_hash_table_create(string_hash, string_eq),
		.cart_ht = ioopm_hash_table_create(int_hash, int_eq),
		.storage_ht = ioopm_hash_table_create(string_hash, string_eq),
		.next_cart_num = 1,
	};
	return store;
}

// Destroys the three hash tables (not what is stored in them)
void ioopm_webstore_destroy(ioopm_webstore_t *store)
{
	ioopm_hash_table_destroy(store->merch_ht);
	ioopm_hash_table_destroy(store->cart_ht);
	ioopm_hash_table_destroy(store->storage_ht);
}