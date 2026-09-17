#include <stdio.h>
#include <stdlib.h>
#include "/home/lillmacke/IOPM/utils.h"
#include <string.h>
#include <ctype.h>

int read_string(char *buf, int buf_siz)
{
    int count = 0;
    int c;

    c = getchar();
    while (c != '\n' && c != EOF && count < buf_siz - 1)
    {
        buf[count] = c;
        count++;
        c = getchar();
    }
    if (count == buf_siz - 1)
    {
        while (c != '\n' && c != EOF)
        {
            c = getchar();
        }
    }
    buf[count] = '\0';
    return count;
}

bool is_number(char *str)
{
    int len = strlen(str);
    if (len == 0 || (str[0] == '-' && len == 1))
    {
        return false;
    }
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]) && !(str[i] == '-' && i == 0))
        {
            return false;
        }
    }
    return true;
}

bool is_float(char *str)
{
    int len = strlen(str);
    if (len == 0 || (str[0] == '-' && len == 1))
    {
        return false;
    }

    int dot_count = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
            {
                return false;
            }
        }
        else if (!isdigit(str[i]) && !(str[i] == '-' && i == 0))
        {
            return false;
        }
    }
    if (dot_count == len || (str[0] == '-' && dot_count == len - 1))
    {
        return false;
    }
    return true;
}

answer_t make_float(char *str)
{
    return (answer_t){.float_value = atof(str)};
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    char buf[255];
    while (true)
    {
        printf("%s\n", question);
        read_string(buf, 255);
        if (check(buf))
        {
            break;
        }
    }
    return convert(buf);
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func *)strdup).string_value;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func *)atoi);
    return answer.int_value; // svaret som ett heltal
}

double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}

void print(char *str)
{
    char *end = str;
    while (*end != '\0') {
        putchar(*end);
        end++;
    }
}
void println(char *str)
{
    print(str);
    putchar('\n');
}

char *trim(char *str)
{
    char *start = str;
    char *end = start + strlen(str) - 1;

    while (isspace(*start))
        ++start;
    while (isspace(*end))
        --end;

    char *cursor = str;
    for (; start <= end; ++start, ++cursor)
    {
        *cursor = *start;
    }
    *cursor = '\0';

    return str;
}