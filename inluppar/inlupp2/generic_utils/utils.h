#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
extern char *strdup(const char *);

/**
 * NOTE: The draft of the documentation for this module was created with Claude (Anthropic).
 * It has been proofread and changed by us afterwards.
 */

typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);


/// @brief Convert a string to an answer_t holding a copy of it
/// @param str the string to copy
/// @return an answer_t with string_value set to a newly allocated copy (caller frees)
answer_t convert_to_string(char *str);

/// @brief Read a line from stdin into a buffer. The rest of an overlong line is discarded
/// @param buf the buffer to read into
/// @param buf_siz the size of the buffer
/// @return the number of characters read, not counting the terminating '\0'
int read_string(char *buf, int buf_siz);

/// @brief Discard characters from stdin up to and including the next newline (or EOF)
void clear_input_buffer(void);

/// @brief Count the characters in a string
/// @param user_string the string to measure
/// @return the length of the string, not counting the terminating '\0'
int string_length(char *user_string);

/// @brief Print a string to stdout
/// @param str the string to print
/// @return 0
int print(char *str);

/// @brief Print a string to stdout, followed by a newline
/// @param str the string to print
/// @return 0
int println(char *str);

/// @brief Check if a string is an integer, i.e. optional leading '-' followed by digits
/// @param str the string to check
/// @return true if str is a number, false otherwise (including for "" and "-")
bool is_number(char *str);

/// @brief Check that a string is not empty
/// @param str the string to check
/// @return true if str has at least one character
bool not_empty(char *str);

/// @brief Ask the user a question until the answer passes a check, then convert it
/// @param question the question to print
/// @param check function deciding whether the input is acceptable
/// @param convert function converting the accepted input to an answer_t
/// @return the converted answer
answer_t ask_question(char *question, check_func *check, convert_func *convert);

/// @brief Ask the user for an integer, repeating until a valid number is entered
/// @param question the question to print
/// @return the entered integer
int ask_question_int(char *question);

/// @brief Ask the user for a non-empty string
/// @param question the question to print
/// @return the entered string, newly allocated (caller frees)
char *ask_question_string(char *question);


#endif