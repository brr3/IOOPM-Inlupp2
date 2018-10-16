#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"
#include "../logic/logic.h"
#include "../logic/elem.h"
#include <stdbool.h>	


static char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

static char ask_question_menu(char *question)
{
  return toupper(*ask_question(question, is_menu_key, (convert_func) strdup).string_value);
}

static int ask_question_check_id(char *question, int merch_count)
{
  int id = ask_question_int(question);
  if (id >= 1 && id <= merch_count)
    {
      return id;
    }
  else
    {
      puts("OBS! A merchandise with that ID does not exist.");
      return ask_question_check_id(question, merch_count);
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
  elem_t elem_key = {.s = item->name};
  elem_t elem_value = {.v = item};
  ioopm_hash_table_insert(items, elem_key, elem_value);
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
  
  int id = ask_question_check_id("Enter the number id of the merchandise you would like to remove:", merch_count); 
  elem_t elem_key = {.s = arr_names[id - 1]};
  elem_t found_element;

  puts("The following merchandise has been removed:");
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t item = *(item_t*) found_element.v;
  print_item(item, id, true);
  
  ioopm_hash_table_remove_entry(items, elem_key);
}

void edit_merch(ioopm_hash_table_t *items) // TODO: Not yet working
{
  list_merch(items);
  int merch_count = ioopm_hash_table_size(items);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_check_id("Enter the number id of the merchandise you would like to edit:", merch_count); 
  elem_t elem_key = {.s = arr_names[id - 1]};
  elem_t found_element;

  puts("You have selected the following merchandise:");
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t item = *(item_t*) found_element.v;
  print_item(item, id, false);

  char *new_name;
  elem_t elem_new_value;
  elem_t elem_new_key;
  while (true) {
  int answer = ask_question_int("What part of the selected merchandise would you like to edit?\n\
[1] Name\n\
[2] Description\n\
[3] Price\n\
[4] Nothing, let me out of here\n");
  switch(answer)
    {
    case 1:
      new_name = ask_question_string("Enter a new name: ");
      while (merch_exists(items, new_name))
        {
          printf("OBS! The name of merchandise you entered already exists in the database.\n");
          new_name = ask_question_string("Enter a different name: ");
        }
      ioopm_hash_table_remove_entry(items, elem_key);
      item_t *new_item = remake_merch(item, new_name);
      elem_new_key.s = new_item->name;
      elem_new_value.v = &new_item;
      ioopm_hash_table_insert(items, elem_new_key, elem_new_value);
      elem_key = elem_new_key;
      break;
    case 2:
      set_item_desc(&item, ask_question_string("Enter a new description: "));
      elem_new_value.v = &item;
      ioopm_hash_table_insert(items, elem_key, elem_new_value);
      break;
    case 3:
      set_item_price(&item, ask_question_int("Enter a new price: "));
      elem_new_value.v = &item;
      ioopm_hash_table_insert(items, elem_key, elem_new_value);
      break;
    case 4:
      return;
    default:
      return;
    }
  }
}

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
          edit_merch(items);
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
