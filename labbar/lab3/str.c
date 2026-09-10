#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

int string_length(char *str)
{
    int count = 0;
    bool end = false;
    while (!end)
    {
        if (str[count] != '\0')
        {
            count++;
        }
        else
            end = true;
    }
    return count;
}

int main(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    print("println(\"");
    print(argv[i]);
    print("\") -> ");
    println(argv[i]);

    print("puts(\"");
    print(argv[i]);
    print("\")     -> ");
    puts(argv[i]);
  }
  return 0;
}