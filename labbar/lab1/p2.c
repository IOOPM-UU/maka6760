#include <stdio.h>

int main(void)
{
    for (int i = 10; i >= 1; i--)
    {                       // loop-kropp (utförs så länge iterationsvillkoret är uppfyllt)
    printf("%d\n", i);    // skriv ut 1, och en radbrytning
  }
  return 0;
}