#include <stdio.h>
#include <stdlib.h>

// Den intressanta delen av programmet
int fib(int num)
{
  int ppf = 0; // the two given fib values
  int pf  = 1;

  for (int i = 1; i < num; ++i)
  {
    int tmp = pf;
    pf = ppf + pf;
    ppf = tmp;
  }

  return pf;
}

int fib_rec(int num) {
    int res = 0;
    if (num == 0) {
        return 0;
    } else if (num == 1) {
        return 1;
    } else {
        return res = fib_rec(num - 1) + fib_rec(num - 2);
    }
}
/// Den ointressanta main()-funktionen
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    int n = atoi(argv[1]);
    printf("fib(%s)     = %d\n", argv[1], fib(n));
    printf("fib_rec(%s) = %d\n", argv[1], fib_rec(n));
  }
  return 0;
}