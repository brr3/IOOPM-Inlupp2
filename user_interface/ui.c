#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/utils.h"
#include "../logic/logic.h"
#include "../logic/elem.h"	



static char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}



static char *ask_question_menu(char *question)
{
  char *key = ask_question(question, is_menu_key, (convert_func) strdup).string_value;
  return key;
}



static char *ask_question_yes_no(char *question)
{
  char *key = ask_question(question, is_yn_key, (convert_func) strdup).string_value;
  return key;
}



static int ask_question_check_nr(char *question, char *error_msg, int *lower_bound, int *upper_bound)
{
  int nr = ask_question_int(question);
  if (lower_bound != NULL && upper_bound == NULL)
    {
      if (nr >= *lower_bound)
        {
          return nr;
        }
      else
        {
          puts(error_msg);
          return ask_question_check_nr(question, error_msg, lower_bound, upper_bound);
        }
    }
  else if (lower_bound == NULL && upper_bound != NULL)
    {
      if (nr <= *upper_bound)
        {
          return nr;
        }
      else
        {
          puts(error_msg);
          return ask_question_check_nr(question, error_msg, lower_bound, upper_bound);
        }
    }
  else if (lower_bound != NULL && upper_bound != NULL)
    {
      if (nr >= *lower_bound && nr <= *upper_bound)
        {
          return nr;
        }
      else
        {
          puts(error_msg);
          return ask_question_check_nr(question, error_msg, lower_bound, upper_bound);
        }
    }
  else
    {
      return nr;
    }
}



static item_t *input_merch(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  return make_merch(name, desc, price);
}



void add_merch(storage_t *storage)
{
  item_t *item = input_merch();
  while (merch_exists(storage, get_item_name(*item)))
    {
      puts("OBS! The name of merchandise you entered already exists in the database.");
      free(item->name);
      set_item_name(item, ask_question_string("Enter a different name: "));
    }
  add_item_to_storage(storage, item);
  puts("Merchandise successfully added!");
}



void list_merch(storage_t *storage, bool print_stock)
{
  int merch_count = ioopm_hash_table_size(storage->items);
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return;
    }
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(storage, arr_names);
  
  int continues = 1;
  for (int i = 0; i < merch_count; i++)
    {
      item_t item = *extract_item_from_storage(storage, arr_names[i], NULL);      

      print_item(item, i + 1, print_stock);

      bool twenty_listings = i == continues * 20 - 1 && merch_count > continues * 20;
      while (twenty_listings)
        {
          char *key = ask_question_yes_no(("Continue listing? (y/n)"));
          char key_up = toupper(*key);
          if (key_up == 'Y') 
            {
              continues++;
              free(key);
              break;
            }
          if (key_up == 'N')
            {
              free(key);
              return;
            }
          free(key);
        }
    }
}



void remove_merch(storage_t *storage)
{
  int merch_count = ioopm_hash_table_size(storage->items);
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return;
    }
  list_merch(storage, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(storage, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to remove:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t elem_value_to_remove;
  item_t *item = extract_item_from_storage(storage, arr_names[id - 1], &elem_value_to_remove);
  
  puts("You have selected the following merchandise:");
  print_item(*item, id, true);

  while (true)
    {
      char *key = ask_question_yes_no(("Are you sure you want to remove the selected merchandise on all its storage locations? (y/n)"));
      char key_up = toupper(*key);
      if (key_up == 'Y')
        {
          remove_item_from_storage(storage, item);
          free(key);
          puts("Merchandise successfully removed!");
          return;
        }
      if (key_up == 'N')
        {
          free(key);
          return;
        }
      free(key);
    }
}



void edit_merch(storage_t *storage)
{
  int merch_count = ioopm_hash_table_size(storage->items);
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return;
    }
  list_merch(storage, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(storage, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to edit:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound); 

  elem_t ignored_value;
  item_t *item = extract_item_from_storage(storage, arr_names[id - 1], &ignored_value);

  while (true)
    {    
      puts("You have selected the following merchandise:");
      print_item(*item, id, false);

      int lower_bound = 1;
      int upper_bound = 4;
      int answer = ask_question_check_nr("What part of the selected merchandise would you like to edit?\n\
[1] Name\n\
[2] Description\n\
[3] Price\n\
[4] Nothing, let me out of here", "OBS! Enter a number between 1 and 4.", &lower_bound, &upper_bound);

      if (answer == 1)
        {
          char *new_name = ask_question_string("Enter a new name, or the same name: ");
          bool same_name = strcmp(new_name, item->name) == 0;          
          while (merch_exists(storage, new_name) && !same_name)
            {
              puts("OBS! A merchandise with the name you entered already exists in the database.");
              free(new_name);
              new_name = ask_question_string("Enter a different name: ");
            }
          remove_item_from_storage(storage, item);
          free(item->name);
          set_item_name(item, new_name);
        }
      else if (answer == 2)
        {
          free(item->desc);
          set_item_desc(item, ask_question_string("Enter a new description: "));
        }
      else if (answer == 3)
        {
          set_item_price(item, ask_question_int("Enter a new price: "));
        }
      else
        {
          remake_merch(storage, item);
          break;
        }
    }
}



void show_stock(storage_t *storage)
{
  int merch_count = ioopm_hash_table_size(storage->items);
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return;
    }
  list_merch(storage, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(storage, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to show the stock from:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  item_t item = *extract_item_from_storage(storage, arr_names[id - 1], NULL);
  print_item(item, id, true);
}



void replenish_stock(storage_t *storage)
{
  ioopm_hash_table_t *items = storage->items;
  int merch_count = ioopm_hash_table_size(items);
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return;
    }
  list_merch(storage, true);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(storage, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to replenish the stock of:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t found_value;
  item_t *item = extract_item_from_storage(storage, arr_names[id - 1], &found_value);

  puts("You have selected the following merchandise:");
  print_item(*item, id, true);

  while (true)
    {
      char *shelf_name = ask_question_shelf("Enter a storage location:\n\
(format: yxx, where y = letter 'A-Z', x = digit 0-9)");

      elem_t elem_shelf_name = {.s = shelf_name};
      bool location_exists = ioopm_hash_table_lookup(storage->locations, elem_shelf_name, &found_value);
      bool names_equal = strcmp(found_value.s, get_item_name(*item)) == 0;

      if (location_exists && names_equal)
        {
          int tmp = 0;
          int *index = &tmp; // Aliaseringsproblem G15
          shelf_t *shelf = find_shelf_in_item_shelves(get_item_shelves(*item), shelf_name, index);
          ioopm_linked_list_remove(get_item_shelves(*item), *index);

          int lower_bound = 1;
          int stock_increase = ask_question_check_nr("Enter how much you want to increase the stock by:", "OBS! Minimum value is 1.", &lower_bound, NULL);
          
          increase_shelf_amount(shelf, stock_increase);
          
          add_shelf_to_item_shelves(get_item_shelves(*item), shelf);
          
          set_item_shelves(item, get_item_shelves(*item));
          add_item_to_storage(storage, item);
        }
      
      if (location_exists && !names_equal)
        {
          puts("OBS! The storage location you entered is already occupied by another item.");
          free(shelf_name);
          continue;
        }
      
      if (!location_exists)
        {                           
          int lower_bound = 1;
          int stock_increase = ask_question_check_nr("Enter how much you want to increase the stock by:", "OBS! Minimum value is 1.", &lower_bound, NULL);
          shelf_t *shelf = make_shelf(shelf_name, stock_increase);

          add_shelf_to_item_shelves(get_item_shelves(*item), shelf);
          
          set_item_shelves(item, get_item_shelves(*item));
          add_item_to_storage(storage, item);

          add_shelf_to_locations(storage, get_shelf_name(*shelf), get_item_name(*item));
        }
      puts("Merchandise successfully replenished!");
      break;
    }
}



void event_loop()
{
  storage_t *storage = make_storage();
  while (true)
    {
      puts("------------------------");
      char *key = ask_question_menu("[A]dd merchandise\n\
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
      char key_up = toupper(*key);
      puts("------------------------");
      if (key_up == 'A')
        {
          add_merch(storage);
        }
      if (key_up == 'L')
        {
          list_merch(storage, false);
        }
      if (key_up == 'R')
        {
          remove_merch(storage);
        }
      if (key_up == 'E')
        {
          edit_merch(storage);
        }
      if (key_up == 'S')
        {
          show_stock(storage);
        }
      if (key_up == 'P')
        {
          replenish_stock(storage);
        }
      if (key_up == 'C')
        {
          
        }
      if (key_up == 'M')
        {
          
        }
      if (key_up == 'D')
        {
          
        }
      if (key_up == 'O')
        {
          
        }
      if (key_up == 'U')
        {
          
        }
      if (key_up == 'K')
        {
          
        }
      if (key_up == 'Q')
        {
          destroy_storage(storage);
          free(key);
          puts("PROGRAM QUIT");
          break;
        }
      free(key);
    }
}
