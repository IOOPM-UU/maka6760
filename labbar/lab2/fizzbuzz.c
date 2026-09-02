#include <stdio.h>
#include <stdlib.h>

void print_number(int num) {
    if ((num % 3 == 0) && (num % 5 == 0)) {
        printf("Fizz Buzz");
    }
    else if (num % 5 == 0) {
        printf("Buzz");
    }
    else if (num % 3 == 0) {
        printf("Fizz");
    }
    else {
        printf("%d", num);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./a.out num1");
        return 0;
    }
    int T = atoi(argv[1]);
    for (int i = 1; i <= T; i++) {
        if (i != T) {
            print_number(i);
            printf(", ");
        }
        else {
            print_number(T);
            printf("\n ");
        }
        
    }
    return 0;
}