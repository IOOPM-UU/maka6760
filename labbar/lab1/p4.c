#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int row = atoi(argv[1]);
  int growth = atoi(argv[2]); 
  int counter = 0;
  for (int i = 1; i <= row; i++)
    {
    for (int j = 1 ; j <= i * growth; j++) 
      {
        counter++;
        printf("%s", "*");
    }
    printf("\n");
  }
  printf("Totalt: %d\n", counter);
  return 0;
}