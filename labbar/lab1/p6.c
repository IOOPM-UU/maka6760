#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
  char *num1 = argv[1];
  char *num2 = argv[2];
  if (argc != 3) {
    printf("Usage: ./a.out num1 num2\n");
    return 0;
  }
  if (!is_number(num1) || !is_number(num2)) {
    printf("%s and/or %s is not a number!\n", num1, num2);
    return 0;
  }

  int tälj = atoi(num1);
  int nämn = atoi(num2);

  tälj = abs(tälj);
  nämn = abs(nämn);

  if (tälj == 0 || nämn == 0) {
    printf("Both numbers must be non-zero!\n");
    return 0;
  }

  while (tälj != nämn) {
    if (tälj < nämn) {
      nämn = nämn - tälj; 
    }
    else if (tälj > nämn) {
      tälj = tälj - nämn; 
    }
  }
  printf("gcd(%s, %s) = %d\n", num1, num2, tälj);
  return 0;
}