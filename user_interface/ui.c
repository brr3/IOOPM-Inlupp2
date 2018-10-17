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

static char ask_question_menu(char *question)
{
  return toupper(*ask_question(question, is_menu_key, (convert_func) strdup).string_value);
}

static char ask_question_yes_no(char *question)
{
  char *key = ask_question(question, is_yn_key, (convert_func) strdup).string_value;
  return toupper(*key);
}

static int ask_question_check_nr(char *question, char *error_msg, int lower_bound, int upper_bound)
{
  int nr = ask_question_int(question);
  if (nr >= lower_bound && nr <= upper_bound)
    {
      return nr;
    }
  else
    {
      puts(error_msg);
      return ask_question_check_nr(question, error_msg, lower_bound, upper_bound);
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
  while (merch_exists(items, item->name))
    {
      puts("OBS! The name of merchandise you entered already exists in the database.");
      item->name = ask_question_string("Enter a different name: ");
    }
  elem_t elem_key = {.s = item->name};
  elem_t elem_value = {.v = item};
  ioopm_hash_table_insert(items, elem_key, elem_value);
}

void list_merch(ioopm_hash_table_t *items, bool print_stock)
{
  // List all items and put all names into a sorted array -->
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  // <--
  
  int continues = 1;
  for (int i = 0; i < merch_count; i++)
    {
      elem_t elem_key = {.s = arr_names[i]};
      // Print item -->
      elem_t found_element;
      ioopm_hash_table_lookup(items, elem_key, &found_element);
      item_t item = *(item_t*) found_element.v; 
      print_item(item, i + 1, print_stock);
      // <--
      
      while (i == continues * 20 - 1 && merch_count > continues * 20)
        {
          char key = ask_question_yes_no(("Continue listing? (y/n)")); // TODO: Fix memory leak
          if (key == 'Y') // If first letter is Y
            {
              continues++;
              break;
            }
          if (key == 'N') // If first letter is N
            {
              return;
            }
        }
    }
}

void remove_merch(ioopm_hash_table_t *items)
// TODO: When storage location hash table is implemented, remove entry from there as well
{
  // List all items, put all names into a sorted array and ask user for ID -->
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to remove:", "OBS! A merchandise with that ID does not exist.", 1, merch_count);
  // <--

  elem_t elem_key = {.s = arr_names[id - 1]};
  // Print selected item -->
  elem_t found_element;
  puts("You have selected the following merchandise:");
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t item = *(item_t*) found_element.v;
  print_item(item, id, true);
  // <--

  while (true)
    {
      char key = ask_question_yes_no(("Are you sure you want to remove the selected merchandise? (y/n)")); // TODO: Fix memory leak
      if (key == 'Y') // If first letter is Y
        {
          break;
        }
      if (key == 'N') // If first letter is N
        {
          return;
        }
    }
  
  // Free -->
  ioopm_hash_table_remove_entry(items, elem_key);
  free_hash_table_keys_values((elem_t) {.v = NULL}, found_element, NULL);
  // <--
}

void edit_merch(ioopm_hash_table_t *items)
{
  // List all items, put all names into a sorted array and ask user for ID -->
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to edit:", "OBS! A merchandise with that ID does not exist.", 1, merch_count); 
  // <--

  elem_t elem_key = {.s = arr_names[id - 1]};
  while (true)
    {
      // Print selected item -->
      elem_t found_element;
      puts("You have selected the following merchandise:");
      ioopm_hash_table_lookup(items, elem_key, &found_element);
      item_t *old_item = (item_t*) found_element.v;
      print_item(*old_item, id, false);
      // <--
      
      int answer = ask_question_int("What part of the selected merchandise would you like to edit?\n\
[1] Name\n\
[2] Description\n\
[3] Price\n\
[4] Nothing, let me out of here");

      if (answer == 1)
        {
          char *new_name = ask_question_string("Enter a new name: ");
          while (merch_exists(items, new_name))
            {
              puts("OBS! The name of merchandise you entered already exists in the database.");
              new_name = ask_question_string("Enter a different name: ");
            }
          ioopm_hash_table_remove_entry(items, elem_key);
          set_item_name(old_item, new_name);
          remake_merch(items, *old_item, &elem_key);
        }
      else if (answer == 2)
        {
          set_item_desc(old_item, ask_question_string("Enter a new description: "));
          elem_t elem_new_value = {.v = old_item};
          ioopm_hash_table_insert(items, elem_key, elem_new_value);
        }
      else if (answer == 3)
        {
          set_item_price(old_item, ask_question_int("Enter a new price: "));
          elem_t elem_new_value = {.v = old_item};
          ioopm_hash_table_insert(items, elem_key, elem_new_value);
        }
      else if (answer == 4)
        {
          break;
        }
      else
        {
          puts("OBS! Enter a number between 1 and 4.");
          continue;
        }
    }
}

void show_stock(ioopm_hash_table_t *items)
{
  // List all items, put all names into a sorted array and ask user for ID -->
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, false);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to show the stock from:", "OBS! A merchandise with that ID does not exist.", 1, merch_count);
  // <--

  elem_t elem_key = {.s = arr_names[id - 1]};

  // Print selected item -->
  elem_t found_element;
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t item = *(item_t*) found_element.v;
  print_item(item, id, true);
  // <--
}

void replenish_stock(ioopm_hash_table_t *items, ioopm_hash_table_t *locations)
{
  // List all items, put all names into a sorted array and ask user for ID -->
  int merch_count = ioopm_hash_table_size(items);
  if (no_merch(merch_count))
    {
      return;
    }
  list_merch(items, true);
  char *arr_names[merch_count];
  ht_names_to_sorted_array(items, arr_names);
  
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to replenish the stock of:", "OBS! A merchandise with that ID does not exist.", 1, merch_count);
  // <--

  elem_t elem_key = {.s = arr_names[id - 1]};
  // Print selected item -->
  elem_t found_element;
  puts("You have selected the following merchandise:");
  ioopm_hash_table_lookup(items, elem_key, &found_element);
  item_t *item = (item_t*) found_element.v;
  print_item(*item, id, true);
  // <--
  
  while (true)
    {
      char *shelf_name = ask_question_shelf("Enter a storage location:\n\
(format: yxx, where y = letter 'A-Z', x = digit 0-9)");

      elem_t elem_shelf_name = {.s = shelf_name};
      bool location_exists = ioopm_hash_table_lookup(locations, elem_shelf_name, &found_element);
      bool names_equal = strcmp(found_element.s, item->name) == 0;
      
      if (location_exists && names_equal)
        {
          ioopm_list_t *item_locations = get_item_shelves(*item);
          int tmp = 0;
          int *index = &tmp;
          shelf_t *shelf = find_shelf_in_list(item_locations, shelf_name, index);
          ioopm_linked_list_remove(item_locations, *index);

          int stock_increase = ask_question_int("Enter how much you want to increase the stock by:");
          increase_shelf_amount(shelf, stock_increase);
          
          elem_t elem_shelf = {.v = shelf};          
          ioopm_linked_list_prepend(item_locations, elem_shelf);
          
          set_item_shelves(item, item_locations);
          elem_t elem_new_value = {.v = item};          
          ioopm_hash_table_insert(items, elem_key, elem_new_value);
        }
      
      if (location_exists && !names_equal)
        {
          puts("OBS! The storage location you entered is already occupied by another item.");
          continue;
        }
      
      if (!location_exists)
        {                           
          ioopm_list_t *item_locations = get_item_shelves(*item);
                    
          int stock_increase = ask_question_int("Enter how much you want to increase the stock by:"); 
          shelf_t *shelf = make_shelf(shelf_name, stock_increase);
          
          elem_t elem_shelf = {.v = shelf};
          ioopm_linked_list_prepend(item_locations, elem_shelf);
          
          set_item_shelves(item, item_locations);
          elem_t elem_new_value = {.v = item};          
          ioopm_hash_table_insert(items, elem_key, elem_new_value);
          
          ioopm_hash_table_insert(locations, elem_shelf_name, elem_key);
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
      puts("------------------------");
      if (key == 'A')
        {
          add_merch(items);
        }
      if (key == 'L')
        {
          list_merch(items, false);
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
          show_stock(items);
        }
      if (key == 'P')
        {
          replenish_stock(items, locations);
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
          ioopm_hash_table_apply_to_all(items, free_hash_table_keys_values, NULL);          
          ioopm_hash_table_destroy(items);
          ioopm_hash_table_destroy(locations);
          puts("PROGRAM QUIT");
          break;
        }
    }
}
