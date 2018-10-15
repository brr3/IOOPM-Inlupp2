#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"
#include "../logic/logic.h"
#include <stdbool.h>	


static char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

static char ask_question_menu(char *question)
{
  return toupper(*ask_question(question, is_menu_key, (convert_func) strdup).string_value);
}

static item_t *input_merch(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  
  return make_merch(name, desc, price);
}

void add_merch(ioopm_hash_table_t *items) // TODO: Add hash table key in ALL caps to prevent multiple occurences of a merch
{
  item_t *item = input_merch();
  while (merch_exists(items, item->name))
    {
      printf("OBS! The name of merchandise you entered already exists in the database.\n");
      item->name = ask_question_string("Enter a different name: ");
    }
  elem_t elem_name = {.s = item->name};
  elem_t elem_item = {.v = item};
  ioopm_hash_table_insert(items, elem_name, elem_item);
}

void list_merch(ioopm_hash_table_t *items) // TODO
{
  int merch_count = ioopm_hash_table_size(items);
  if (merch_count == 0)
    {
      puts("No merchandise has been added to the warehouse yet!");
      return;
    }
  ioopm_hash_table_apply_to_all(items, print_item_apply_func, NULL);
}

/*item_t remove_merch(ioopm_hash_table_t *items, char *merch) // TODO
{ 
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
          print_item(items[editpos - 1]);         

          for (int i = editpos - 1; i <= no_items - 1; i++)
            {
              items[i] = items[i+1];
            }
          return *items;
        }
    }  
} */

/*item_t edit_db(item_t *items, int no_items) // TODO
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
          print_item(items[editpos - 1]);
       
          items[editpos - 1] = input_merch();
          return *items;
        }
    }
}*/

void event_loop()
{
  ioopm_hash_table_t *items = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  while (true)
    {
      printf("--------------------- \n");
      char key = ask_question_menu("[A]dd merchandise\n\
[L]ist merchandise\n\
[R]emove merchandise\n\
[E]dit merchandise\n\
[S]how stock of merchandise\n\
Re[p]lenish stock of merchandise\n\
[C]reate shopping cart\n\
Re[m]ove shopping cart\n\
Ad[d] item to shopping cart\n\
Rem[o]ve item from shopping cart\n\
Calc[u]late cost\n\
Chec[k]out\n\
[Q]uit\n");
      printf("--------------------- \n");
      if (key == 'A')
        {
          add_merch(items);
        }
      if (key == 'L')
        {
          list_merch(items);
        }
      if (key == 'R')
        {
          //remove_merch(items);
        }
      if (key == 'E')
        {
          //edit_merch(items);
        }
      if (key == 'S')
        {
          //show_merch_stock(items);
        }
      if (key == 'P')
        {
          //replenish_merch_stock(items);
        }
      if (key == 'C')
        {
          
        }
      if (key == 'M')
        {
          
        }
      if (key == 'D')
        {
          
        }
      if (key == 'O')
        {
          
        }
      if (key == 'U')
        {
          
        }
      if (key == 'K')
        {
          
        }
      if (key == 'Q')
        {
          puts("PROGRAM QUIT");
          break;
        }
    }
}
