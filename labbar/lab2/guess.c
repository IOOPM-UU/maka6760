#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(void) {
    int T = rand() % 1024;

    char name[255];
    ask_question_string("Skriv in ditt namn: ", name, 255);

    printf("Du %s, jag tänker på ett tal ... kan du gissa vilket?\n", name);

    int guesses = 0;
    bool won = false; 

    while (guesses <= 15) {
        int guess = ask_question_int("Gissa ett tal: ");
        guesses++;

        if (guess < T) {
            printf("För litet!\n");
        }
        else if (guess > T) {
            printf("För stort!\n");
        }
        else {
            printf("Bingo!\n");
            won = true;
            break;
        }
    }

    if (won) {
        printf("Det tog %s %d gissningar att komma fram till %d\n", name, guesses, T);
    }
    else {
        printf("Nu har du slut på gissningar! Jag tänkte på %d\n", T);
    }
    return 0;
}