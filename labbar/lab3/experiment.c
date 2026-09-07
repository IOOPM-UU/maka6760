#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

extern char *strdup(const char *);

typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

answer_t ask_question(char *question, check_func *check, convert_func *convert) {
    char buf[255];
    while (true) {
        printf("%s\n", question);
        read_string(buf, 255);
        if (check(buf)) {
            break;
        }
    }
    return convert(buf);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; // svaret som ett heltal
}

answer_t make_float(char *str)
{
  return (answer_t) { .float_value = atof(str) };
}

double ask_question_float(char *question)
{
  return ask_question(question, is_float, make_float).float_value;
}


bool not_empty(char *str)
{
  return strlen(str) > 0;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}


int main(int arc, char *argv[]) {

}

