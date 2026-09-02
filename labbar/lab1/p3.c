#include <stdio.h>

int main(void)
{
  int counter = 0;
  for (int i = 1; i <= 10; i++)
    {
    for (int j = 1; j <= i; j++) 
      {
        counter++;
        printf("%s", "*");
    }
    printf("\n");
  }
  printf("Totalt: %d\n", counter);
  return 0;
}