#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_funcs.h"

size_t string_hash(elem_t key)
{
	size_t result = 0;
	for (char *str = key.s; *str != '\0'; str++)
	{
		result = result * 31 + (unsigned char)*str;
	}
	return result;
}

bool string_eq(elem_t a, elem_t b)
{
	return strcmp(a.s, b.s) == 0;
}

size_t int_hash(elem_t key)
{
	return key.i;
}

bool int_eq(elem_t a, elem_t b)
{
	return a.i == b.i;
}