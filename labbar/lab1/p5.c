#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int result = 0;
  int N = atoi(argv[1]);

  if (N == 0) {
    result = 1;
  }

  for (int i = 2; i * i <= N; i++) {
    if (N % i == 0) {
      result = 1;
    }
    else {
      result = 0;
    }
  }
  if (result == 0) {
    printf("%d is a prime number\n", N);
  }
  else {
    printf("%d is not a prime number\n", N);   
  }
  return 0;
}