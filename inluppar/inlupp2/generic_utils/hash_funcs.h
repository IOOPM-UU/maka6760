#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

/// @brief String hash function used in hash tables
/// @param key element whose string member (s) is hashed
/// @return the hash value of the string
size_t string_hash(elem_t key);

/// @brief String equality function used in hash tables
/// @param a element holding the first string
/// @param b element holding the second string
/// @return true if the strings are equal
bool string_eq(elem_t a, elem_t b);

/// @brief Integer hash function used in hash tables
/// @param key element whose int member (i) is hashed
/// @return the hash value of the integer
size_t int_hash(elem_t key);

/// @brief Integer equality function used in hash tables
/// @param a element holding the first integer
/// @param b element holding the second integer
/// @return true if the integers are equal
bool int_eq(elem_t a, elem_t b);
