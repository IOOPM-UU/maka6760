#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

void print(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
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