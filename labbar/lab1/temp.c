#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Ska ta emot en sträng och returnera true om strängen
// är ett positivt eller negativt tal, annars false
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

int main(int argc, char *argv[]) {
    if (argc > 1 && is_number(argv[1])) {
        printf("%s is a number\n", argv[1]);
    }
    else {
        if (argc > 1) {
            printf("%s is not a number\n", argv[1]);
        }
        else {
            printf("Please provide a command line argument!\n");
        }
    }

}