#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../utils.h"

struct item
{
    char *name;
    char *desc;
    int price;
    char *shelf;
};
typedef struct item item_t;

void print_item(item_t *item)
{
    printf("Name: %s\nDesc: %s\nPrice: %d.%02d\nShelf: %s\n", item->name,
           item->desc, (item->price) / 100, (item->price) % 100, item->shelf);
}

item_t make_item(char *n, char *d, int p, char *s)
{
    item_t item = {.name = n, .desc = d, .price = p, .shelf = s};
    return item;
}

bool is_shelf(char *shelf)
{
    int len = strlen(shelf);
    if (len < 2 || !isalpha(shelf[0]))
    {
        return false;
    }
    for (int i = 1; i < len; i++)
    {
        if (!isdigit(shelf[i]))
        {
            return false;
        }
    }
    return true;
}

char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func *)strdup).string_value;
}
item_t input_item(void)
{
    char *name = ask_question_string("Namn på varan: ");
    char *desc = ask_question_string("Beskrivning av varan: ");
    int price = ask_question_int("Varans pris: ");
    char *shelf = ask_question_shelf("Varans hyllplats: ");

    item_t item = { .name = name, .desc = desc, .price =price, .shelf = shelf};
    return item;
}

char *magick(char *a1[], char *a2[], char *a3[], int len) {

    char buf[255];
    buf[0] = '\0';

    int T1 = rand() % len;
    int T2 = rand() % len;
    int T3 = rand() % len;

    strcat(buf, a1[T1]);
    strcat(buf, "-");
    strcat(buf, a2[T2]);
    strcat(buf, " ");
    strcat(buf, a3[T3]);

    return strdup(buf); 
}

void list_db(item_t *items, int no_items) 
{
    for (int i = 0; i < no_items; i++) 
    {
        printf("%d. %s\n", i + 1, items[i].name);
    }
}

void edit_db(item_t *db, int db_siz) 
{
    int item_number = ask_question_int("Vilken vara vill du ändra?");

    if (item_number < 1 ||  item_number > db_siz)
    {
        printf("det finns ingen vara med nummer %d.\n", item_number);
        return;
    }
    int index = item_number - 1;

    print_item(&db[index]);

    db[index] = input_item();
}

void print_menu(void) 
{
    puts("[L]ägga till en vara\n"
        "[T]a bort en vara\n"
        "[R]edigera en vara\n"
        "Ån[g]ra senaste ändringen\n"
        "Lista [h]ela varukatalogen\n"
        "[A]vsluta");
}

bool is_menu_choice(char *str) 
{
    return strlen(str) == 1 && strchr("LlTtRrGgHhAa", str[0]) != NULL;
}

char ask_question_menu(void)
{
    char buf[255];
    int len;

    do
    {
        print_menu();
        len = read_string(buf, 255);
    } while (!is_menu_choice(buf));

    return toupper(buf[0]);
    
}

void add_item_to_db(item_t *db, int *db_siz) 
{
    item_t item = input_item();
    db[*db_siz] = item;
    ++(*db_siz);
}

void remove_item_from_db(item_t *db, int *db_siz)
{
    list_db(db, *db_siz);

    int choice = ask_question_int("Vilken vara vill du ta bort? (ange nummer)");
    int index = choice - 1; // konvertera från 1-baserat till 0-baserat

    if (index < 0 || index >= *db_siz)
    {
        puts("Ogiltigt val.");
        return;
    }

    for (int i = index; i < *db_siz - 1; ++i)
    {
        db[i] = db[i + 1];
    }

    --(*db_siz);
}

void event_loop(item_t *db, int *db_siz)
{
    bool running = true;

    while (running)
    {
        char choice = ask_question_menu();

        switch (choice)
        {
            case 'L':
                if (*db_siz < 16)
                {
                    add_item_to_db(db, db_siz);
                }
                else
                {
                    puts("Databasen är full!");
                }
                break;

            case 'T':
                remove_item_from_db(db, db_siz);
                break;

            case 'R':
                edit_db(db, *db_siz);
                break;

            case 'G':
                puts("Inte implementerad än..");
                break;

            case 'H':
                list_db(db, *db_siz);
                break;

            case 'A':
                running = false;
                break;
        }
    }
}

int main(int argc, char *argv[])
{
  char *array1[] = {
      "Laser", "Polka", "Extra", "Turbo", "Kosmisk", "Arktisk",
      "Kvant", "Elastisk", "Magnetisk", "Radioaktiv", "Vintage",
      "Dynamisk", "Cyber", "Mystisk", "Episk", "Ultra"
  };

  char *array2[] = {
      "förnicklad", "smakande", "ordinär", "handgjord", "självlysande",
      "fjärrstyrd", "vattentät", "eldriven", "bärbar", "oidentifierbar",
      "vegansk", "ekologisk", "svårförklarlig", "tveksam", "opålitlig",
      "överdimensionerad"
  };

  char *array3[] = {
      "skruvdragare", "kola", "uppgift", "träningsvärk", "grillspett",
      "läskedryck", "existenskris", "morgonrock", "diskmaskin",
      "kaffekopp", "bokhylla", "cykelpump", "brevlåda", "yoghurt",
      "handduk", "paraply"
  };

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16];
    int db_siz = 0;

    int items = atoi(argv[1]);

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1;
    }

    for (int i = db_siz; i < 16; ++i)
    {
      char *name = magick(array1, array2, array3, 16);
      char *desc = magick(array1, array2, array3, 16);
      int price = random() % 200000;
      char shelf[] = { random() % ('Z'-'A') + 'A',
                       random() % 10 + '0',
                       random() % 10 + '0',
                       '\0' };
      item_t item = make_item(name, desc, price, strdup(shelf));

      db[db_siz] = item;
      ++db_siz;
    }

    list_db(db, db_siz);

    event_loop(db, &db_siz);
  }
  return 0;
}