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
      puts("INCORRECT USAGE ERROR!!!");
      assert(false);
    }
}


static item_t *input_merch(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  return make_merch(name, desc, price);
}


void add_merch(ioopm_hash_table_t *items)
// TODO: Add hash table key in ALL caps to prevent multiple occurences of a merchandise
{
  item_t *item = input_merch();
  while (merch_exists(items, get_item_name(*item)))
    {
      puts("OBS! The name of merchandise you entered already exists in the database.");
      set_item_name(item, ask_question_string("Enter a different name: "));
    }
  add_item_to_storage(items, item);
}


void list_merch(ioopm_hash_table_t *items, bool print_stock)
{
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(items, arr_names);
  
  int continues = 1;
  for (int i = 0; i < merch_count; i++)
    {
      item_t item = *extract_item_from_storage(items, i, arr_names, NULL);      

      print_item(item, i + 1, print_stock);
      
      while (i == continues * 20 - 1 && merch_count > continues * 20)
        {
          char *key = ask_question_yes_no(("Continue listing? (y/n)"));
          char key_up = toupper(*key);
          if (key_up == 'Y') 
            {
              continues++;
              break;
            }
          if (key_up == 'N')
            {
              return;
            }
          free(key);
        }
    }
}


void remove_merch(ioopm_hash_table_t *items)
{
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(items, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to remove:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t ignored_value;
  item_t *item = extract_item_from_storage(items, id - 1, arr_names, &ignored_value);
  
  puts("You have selected the following merchandise:");
  print_item(*item, id, true);

  while (true)
    {
      char *key = ask_question_yes_no(("Are you sure you want to remove the selected merchandise? (y/n)"));
      char key_up = toupper(*key);
      if (key_up == 'Y')
        {
          break;
        }
      if (key_up == 'N')
        {
          return;
        }
    }
  remove_item_from_storage(items, item);
  puts("Merchandise successfully removed!");
}


void edit_merch(ioopm_hash_table_t *items)
{
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(items, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to edit:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound); 

  elem_t ignored_value;
  item_t *item = extract_item_from_storage(items, id - 1, arr_names, &ignored_value);

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
          char *new_name = ask_question_string("Enter a new name: ");
          while (merch_exists(items, new_name))
            {
              puts("OBS! The name of merchandise you entered already exists in the database.");
              new_name = ask_question_string("Enter a different name: ");
            }
          remove_item_from_storage(items, item);
          free(item->name);
          set_item_name(item, new_name);
        }
      else if (answer == 2)
        {
          free(item->desc);
          set_item_desc(item, ask_question_string("Enter a new description: "));
          add_item_to_storage(items, item);
        }
      else if (answer == 3)
        {
          set_item_price(item, ask_question_int("Enter a new price: "));
          add_item_to_storage(items, item);
        }
      else
        {
          remake_merch(items, item);
          break;
        }
    }
}


void show_stock(ioopm_hash_table_t *items)
{
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(items, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to show the stock from:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  item_t item = *extract_item_from_storage(items, id - 1, arr_names, NULL);
  print_item(item, id, true);
}


void replenish_stock(ioopm_hash_table_t *items, ioopm_hash_table_t *locations)
{
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, true);
  
  char *arr_names[merch_count];
  storage_names_to_sorted_array(items, arr_names);

  int lower_bound = 1;
  int upper_bound = merch_count;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to replenish the stock of:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t found_value;
  item_t *item = extract_item_from_storage(items, id - 1, arr_names, &found_value);
  
  puts("You have selected the following merchandise:");
  print_item(*item, id, true);

  while (true)
    {
      char *shelf_name = ask_question_shelf("Enter a storage location:\n\
(format: yxx, where y = letter 'A-Z', x = digit 0-9)");

      elem_t elem_shelf_name = {.s = shelf_name};
      bool location_exists = ioopm_hash_table_lookup(locations, elem_shelf_name, &found_value);
      bool names_equal = strcmp(found_value.s, get_item_name(*item)) == 0;
      
      if (location_exists && names_equal)
        {
          ioopm_list_t *item_locations = get_item_shelves(*item);
          int tmp = 0;
          int *index = &tmp;
          shelf_t *shelf = find_shelf_in_list(item_locations, shelf_name, index);
          ioopm_linked_list_remove(item_locations, *index);

          int lower_bound = 1;
          int stock_increase = ask_question_check_nr("Enter how much you want to increase the stock by:", "OBS! Minimum value is 1.", &lower_bound, NULL);
          
          increase_shelf_amount(shelf, stock_increase);
          
          add_shelf_to_list(item_locations, shelf);
          
          set_item_shelves(item, item_locations);
          add_item_to_storage(items, item);
        }
      
      if (location_exists && !names_equal)
        {
          puts("OBS! The storage location you entered is already occupied by another item.");
          continue;
        }
      
      if (!location_exists)
        {                           
          ioopm_list_t *item_locations = get_item_shelves(*item);

          int lower_bound = 1;
          int stock_increase = ask_question_check_nr("Enter how much you want to increase the stock by:", "OBS! Minimum value is 1.", &lower_bound, NULL);
          shelf_t *shelf = make_shelf(shelf_name, stock_increase);
          
          add_shelf_to_list(item_locations, shelf);
          
          set_item_shelves(item, item_locations);
          add_item_to_storage(items, item);

          add_shelf_to_ht(locations, shelf);
        }      
      puts("Merchandise successfully replenished!");
      break;
    }
}


void event_loop()
{
  ioopm_hash_table_t *items = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  ioopm_hash_table_t *locations = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
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
          add_merch(items);
        }
      if (key_up == 'L')
        {
          list_merch(items, false);
        }
      if (key_up == 'R')
        {
          remove_merch(items);
        }
      if (key_up == 'E')
        {
          edit_merch(items);
        }
      if (key_up == 'S')
        {
          show_stock(items);
        }
      if (key_up == 'P')
        {
          replenish_stock(items, locations);
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
          ioopm_hash_table_apply_to_all(items, free_hash_table_keys_values, NULL);          
          ioopm_hash_table_destroy(items);
          ioopm_hash_table_destroy(locations);
          free(key);
          puts("PROGRAM QUIT");
          break;
        }
      free(key);
    }
}
