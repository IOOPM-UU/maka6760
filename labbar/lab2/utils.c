#include <stdio.h>
#include "utils.h"

int ask_question_int(char *question) {
    int result = 0;
    int conversions;
    do {
        printf("%s\n", question);
        conversions = scanf("%d", &result);
        int c;
        do {
            c = getchar();
        } 
        while (c != '\n' && c != EOF);
        putchar('\n');
        
    } 
    while (conversions < 1);
    return result;
}

int read_string(char *buf, int buf_siz) {
    int count = 0;
    int c;

    c = getchar();
    while (c != '\n' && c != EOF && count < buf_siz - 1) {
        buf[count] = c;
        count++;
        c = getchar();
    }
    if (count == buf_siz -1) {
        while (c != '\n' && c != EOF) {
            c = getchar();
        }
    }
    buf[count] = '\0';
    return count;
       
}

char *ask_question_string(char *question, char *buf, int buf_siz) {
    int len;
    do
    {
        printf("%s\n", question);
        len = read_string(buf, buf_siz);
    } while (len == 0);
    return buf;  
}

bool is_number(char *str) {
    int len = strlen(str);
    if (len == 0 || (str[0] == '-' && len == 1)) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i]) && !(str[i] == '-' && i == 0)) {
            return false;
        }
    }
    return true;
}