#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"




typedef struct item item_t;

struct item
{
  char *name;
  char *desc;
  int price;
  char *shelf;
  int amount;
};




void print_item(item_t *i)
{
  int kr = (i->price/100);
  int ore = (i->price) % 100;
  printf("--------------------- \n");
  printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelf: %s\nAmount: %d\n", i->name, i->desc, kr, ore, i->shelf, i->amount);
}


item_t make_item(char *name, char *desc, int price, char *shelf, int amount)
{
  item_t t = { .name = name, .desc = desc, .price = price, .shelf = shelf, .amount = amount};
  return t;
}


bool is_shelf(char *shelf)
{
  char *str = shelf;
  if (isdigit(*str))
    {
      return false;
    }
  ++str;
  while (*str != '\0')
    {
      if (!isdigit(*str))
      {
        return false;
      }
    ++str;
    }
  --str;
  if (!isdigit(*str))
    {
      return false;
    }
  return true;
}


bool is_menu_key(char *key)
{
  switch(*key)
    {
    case 'L':
    case 'l':
    case 'T':
    case 't':
    case 'R':
    case 'r':
    case 'G':
    case 'g':
    case 'H':
    case 'h':
    case 'A':
    case 'a':
      return true;
    default:
      return false;
    }
}


char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}


char ask_question_menu(char *question)
{
  return toupper(*ask_question(question, is_menu_key, (convert_func) strdup).string_value);
}


item_t input_item(void)
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  char *shelf = ask_question_shelf("Enter a shelf: ");
  int amount = ask_question_int("Enter amount: ");
  
  return make_item(name, desc, price, shelf, amount);
}


char *magick(char *arr1[], char *arr2[], char *arr3[], int size)
{
  char buf[255];
  char *temp1 = arr1[(random() % size)];
  char *temp2 = arr2[(random() % size)];
  char *temp3 = arr3[(random() % size)];
  
  int le = strlen(temp1);
  int le2 = strlen(temp2);
  int le3 = strlen(temp3);
  int wcounter = 0;
  int z = 0;
  
  for (int i = 0; i <= le; ++i)
    {
      if (wcounter == 0)
        {
          buf[i] = temp1[z];
          //printf("Ord 1: %s, i: %d\n", buf, i);
        } else if (wcounter == 1)
        {
          buf[i] = temp2[z];
          //printf("Ord 2: %s, i: %d\n", buf, i);
        } else
        {
          buf[i] = temp3[z];
          //printf("Ord 3: %s, i: %d\n", buf, i);
        }
      ++z;
      if (i == le)
        {
          if (wcounter == 0)
            {
              buf[le] = '-';
              le += le2 + 1;
            } else if (wcounter == 1)
            {
              buf[le] = ' ';
              le += le3 + 1;
            } else
            {
              buf[le] = '\0';
            }
          ++wcounter;
          z = 0;
        }
    }
  return strdup(buf);
}


void list_db(item_t *items, int no_items)
{
  char *name = items->name;

  for (int i = 0; i < no_items; i++)
  {
    name = items[i].name;
    printf("%d. %s\n", i + 1, name);
  }
}	


item_t edit_db(item_t *items, int no_items)
{
  printf("--------------------- \n");
  list_db(items, no_items);

  while (true)
    {
      int editpos = ask_question_int("Enter the number of an item to edit: ");

      if (editpos < 1 || editpos > no_items)
        {
          printf("OBS! Enter a number between 1 and %d.\n", no_items);
        } else
        {
          printf("--------------------- \n");
          printf("You have selected the following item: \n");
          print_item(&items[editpos - 1]);
       
          items[editpos - 1] = input_item();
          return *items;
        }
    }
}


item_t add_item_to_db(item_t *items, int no_items)
{
  items[no_items] = input_item();
  return *items;
}


item_t remove_item_from_db(item_t *items, int no_items)
{ 
  printf("--------------------- \n");
  list_db(items, no_items);

  while (true)
    {
      int editpos = ask_question_int("Enter the number of the item you want to remove: ");

      if (editpos < 1 || editpos > no_items)
        {
          printf("OBS! Enter a number between 1 and %d.\n", no_items); 
        } else
        {
          printf("--------------------- \n");
          printf("The following item has now been removed: \n");
          print_item(&items[editpos - 1]);         

          for (int i = editpos - 1; i <= no_items - 1; i++)
            {
              items[i] = items[i+1];
            }
          return *items;
        }
    }  
}


void event_loop(item_t *items, int no_items, int max_items)
{
  while (true)
    {
      printf("--------------------- \n");

      char key = ask_question_menu("[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista [h]ela varukatalogen\n\
[A]vsluta\n");

      if (key == 'L')
        {
          if (no_items < max_items)
            {
              *items = add_item_to_db(items, no_items);
              ++no_items;
            } else
            {
              printf("Max number of entries reached!\n");
            }
        }
      if (key == 'T')
        {         
          if (no_items < 1)
            {
              printf("--------------------- \n");
              printf("OBS! No items exist in database.\n");
            } else
            {
              *items = remove_item_from_db(items, no_items);
              --no_items;
            }
        }
      if (key == 'R')
        {
          if (no_items < 1)
            {
              printf("--------------------- \n");
              printf("OBS! No items exist in database.\n");
            } else
            {
              *items = edit_db(items, no_items);
            }
        }
      if (key == 'G')
        {
          printf("--------------------- \n");
          printf("Not yet implemented!\n");         
        }
      if (key == 'H')
        {
          if (no_items < 1)
            {
              printf("--------------------- \n");
              printf("OBS! No items exist in database.\n");
            } else
            {
              list_db(items, no_items);
            }
        }
      if (key == 'A')
        {
          printf("--------------------- \n");
          printf("Program Quit!\n");
          printf("--------------------- \n");
          break;
        }     
    }
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    {
      printf("OBS! Array size argument missing.\n");
      return 0;
    }
  
  int max_items = atoi(argv[1]);
  item_t db[max_items];                                                                                    
  int db_current_size = 0; 
  char key = toupper(*ask_question_string("Add random entries to the database? (y/n)\n"));
  
  if (key == 'Y')
    {
      char *array1[] = { "Laser",        "Polka",    "Extra" };                                                                           
      char *array2[] = { "förnicklad",   "smakande", "ordinär" };                                                                        
      char *array3[] = { "skruvdragare", "kola",     "uppgift" };

      for (int i = db_current_size; i < max_items; ++i)                                                                                     
        {                                                                                                                                   
          char *name = magick(array1, array2, array3, 3);                                                                                   
          char *desc = magick(array1, array2, array3, 3);                                                                                  
          int price = random() % 200000;                                                                                                    
          char shelf[] = { random() % ('Z'-'A') + 'A',                                                                                      
                           random() % 10 + '0',                                                                                             
                           random() % 10 + '0',                                                                                             
                           '\0' };
          int amount = random() % 2000;
          item_t item = make_item(name, desc, price, shelf, amount);

          db[db_current_size] = item;                                                                                                       
          ++db_current_size;                                                                                                                
        }
      event_loop(db, db_current_size, max_items);
    } else
    {
      event_loop(db, db_current_size, max_items);
    }
}
