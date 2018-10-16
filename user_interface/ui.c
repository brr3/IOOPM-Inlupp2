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

static int ask_question_remove(char *question, int merch_count)
{
  int id = ask_question_int(question);
  if (id >= 1 && id <= merch_count)
    {
      return id;
    }
  else
    {
      puts("OBS! A merchandise with that ID does not exist.");
      return ask_question_remove(question, merch_count);
    }
}

static item_t *input_merch(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  
  return make_merch(name, desc, price);
}

void add_merch(ioopm_hash_table_t *items) // TODO: Add hash table key in ALL caps to prevent multiple occurences of a merchandise
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

void list_merch(ioopm_hash_table_t *items)
{
  int merch_count = ioopm_hash_table_size(items);
  if (merch_count == 0)
    {
      puts("No merchandise has been added to the warehouse yet!");
      return;
    }
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  elem_t found_element;
  int continues = 1;
  for (int i = 0; i < merch_count; i++)
    {
      elem_t elem_name  = {.s = arr_names[i]};
      ioopm_hash_table_lookup(items, elem_name, &found_element);
      item_t item = *(item_t*) found_element.v;
      
      print_item(item, i + 1, false);
      
      while (i == continues * 20 - 1 && merch_count > continues * 20)
        {
          char key = toupper(*ask_question_string("Continue listing? (y/n)")); // TODO: Fix memory leak
          if (key == 'Y') // If first letter is Y
            {
              continues++;
              break;
            }
          if (key == 'N') // If first letter is N
            {
              i = merch_count;
              break;
            }
        }
    }
}

void remove_merch(ioopm_hash_table_t *items) // TODO: When storage location hash table is done, remove entry from there as well
{ 
  list_merch(items);
  int merch_count = ioopm_hash_table_size(items);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_remove("Enter the number id of the merchandise you would like to remove:", merch_count); 
  elem_t elem_key = {.s = arr_names[id - 1]};
  elem_t found_element;

  puts("The following merchandise has been removed:");
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t item = *(item_t*) found_element.v;
  print_item(item, id, true);
  
  ioopm_hash_table_remove_entry(items, elem_key);
}

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

static void free_hash_table_values_keys(elem_t key_ignored, elem_t value, void *x_ignored)
{
  free((*(item_t*) value.v).name);
  free((*(item_t*) value.v).desc);
  ioopm_linked_list_destroy((*(item_t*) value.v).shelves);
  free(value.v);
}

void event_loop()
{
  ioopm_hash_table_t *items = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  while (true)
    {
      printf("---------------------\n");
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
[Q]uit\n"); // TODO: Fix memory leak
      printf("---------------------\n");
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
          remove_merch(items);
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
          ioopm_hash_table_apply_to_all(items, free_hash_table_values_keys, NULL);          
          ioopm_hash_table_destroy(items);
          puts("PROGRAM QUIT");
          break;
        }
    }
}
