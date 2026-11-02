#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/// @brief Convert a string to an int
static answer_t convert_to_int(char *str)
{
    return (answer_t) { .int_value = atoi(str) };
}

answer_t convert_to_string(char *str)
{
    return (answer_t) { .string_value = strdup(str) };
}

void clear_input_buffer(void)
{
    int c;

    do
    {
        c = getchar();
    } 
    while (c != '\n' && c != EOF);

    return;
}

int read_string(char *buf, int buf_siz)
{
    int counter = 0;

    for (int i = 0; i < buf_siz - 1; i += 1)
    {
        int current_character = getchar();

        if (current_character == '\n' || current_character == EOF)
        {
            buf[i] = '\0';
            return counter;
        }
        else
        {
            buf[i] = current_character;
            counter += 1;
        }

    }

    buf[255] = '\0';
    clear_input_buffer();
    return counter;
}

int string_length(char *user_string)
{
    int i = 0;

    if (user_string[i] == '\0')
    {
        return i;
    }

    else
    {
        do
        {
            i += 1;
        }
        while(user_string[i] != '\0');
    }

    return i;
}

int print(char *str)
{
    int str_length = string_length(str);
    for (int i = 0; i < str_length; i += 1)
    {
        putchar(str[i]);
    }

    return 0;
}

int println(char *str)
{
    print(str);
    print("\n");

    return 0;
}

bool is_number(char *str)
{
    int length = strlen(str);

    if (length == 0)
    {
        return false;
    }

    for (int i = 0; i < length; i += 1)
    {
        if (i == 0)
        {  
            char first_character = str[0];
            
            if (length == 1 && first_character == '-')
            {
                return false;
            }

            bool check_first_character = isdigit(first_character) || first_character == '-';
            if (!check_first_character)
            {
                return false;
            }
        }
        else
        {
            int character = str[i];
            if (!isdigit(character))
            {
                return false;
            }
        }
    }

    return true;
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    char buf[255];
    bool correct_format = false;

    do
    {
        printf("%s\n", question);
        read_string(buf, 255);
        correct_format = check(buf);
        
    }
    while(!correct_format);

    answer_t result = convert(buf);
    return result;
}


int ask_question_int(char *question)
{
    return ask_question(question, is_number, convert_to_int).int_value;
}


char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, convert_to_string).string_value;
}