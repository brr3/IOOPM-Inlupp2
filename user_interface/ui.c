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



static bool storage_null(void *storage)
{
  if (storage == NULL)
    {
      puts("OBS! Storage not initialised.");
      return true;
    }
  else
    {
      return false;
    }
}



static item_t *input_item(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  return make_item(name, desc, price);
}

void add_item(storage_t *storage)
{
  if (storage_null(storage)) return;
  
  item_t *item = input_item();
  while (item_exists(*storage, get_item_name(*item))) 
    {
      puts("OBS! The name of merchandise you entered already exists in the database.");
      set_item_name(item, ask_question_string("Enter a different name: "));
    }
  add_item_to_storage(storage, item);
  puts("Merchandise successfully added!");
}



static bool storage_empty(storage_t *storage)
{
  if (get_storage_items_amount(*storage) == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return true;
    }
  else
    {
      return false;
    }
}



void list_items(storage_t *storage, bool print_stock)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  
  int item_amount = get_storage_items_amount(*storage); 
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);
 
  int continues = 1;
  for (int i = 0; i < item_amount; i++)
    {
      item_t item = *extract_item_from_storage(*storage, arr_names[i], NULL);      

      print_item(item, i + 1, print_stock);

      bool twenty_listings = i == continues * 20 - 1 && item_amount > continues * 20;
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



void remove_item(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  
  list_items(storage, false);

  int item_amount = get_storage_items_amount(*storage);
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);

  int lower_bound = 1;
  int upper_bound = item_amount;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to remove:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t elem_value_to_remove;
  item_t *item = extract_item_from_storage(*storage, arr_names[id - 1], &elem_value_to_remove);
  
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



void edit_item(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  
  list_items(storage, false);

  int item_amount = get_storage_items_amount(*storage);
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);

  int lower_bound = 1;
  int upper_bound = item_amount;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to edit:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound); 

  elem_t ignored_value;
  item_t *item = extract_item_from_storage(*storage, arr_names[id - 1], &ignored_value);
  bool item_name_modified = false;
  bool anything_modified = false;

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
          bool same_name = strcmp(new_name, get_item_name(*item)) == 0;          
          while (item_exists(*storage, new_name) && !same_name)
            {
              puts("OBS! A merchandise with the name you entered already exists in the database.");
              free(new_name);
              new_name = ask_question_string("Enter a different name: ");
            }
          remove_item_from_storage(storage, item);         
          set_item_name(item, new_name);
          item_name_modified = true;
          anything_modified = true;
        }
      else if (answer == 2)
        {
          set_item_desc(item, ask_question_string("Enter a new description: "));
          anything_modified = true;
        }
      else if (answer == 3)
        {
          set_item_price(item, ask_question_int("Enter a new price: "));
          anything_modified = true;
        }
      else
        {
          if (anything_modified)
            {
              if (item_name_modified)
                {
                  remake_item(storage, item);
                }
              else
                {
                  add_item_to_storage(storage, item);
                }
              break;
            }
          else
            {
              break;
            }
        }
    }
}



void show_item_stock(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  
  list_items(storage, false);

  int item_amount = get_storage_items_amount(*storage);
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);

  int lower_bound = 1;
  int upper_bound = item_amount;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to show the stock from:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  item_t item = *extract_item_from_storage(*storage, arr_names[id - 1], NULL);
  print_item(item, id, true);
}



void replenish_item_stock(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  
  list_items(storage, true);

  int item_amount = get_storage_items_amount(*storage);
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);

  int lower_bound = 1;
  int upper_bound = item_amount;
  int id = ask_question_check_nr("Enter the number id of the merchandise you would like to replenish the stock of:", "OBS! A merchandise with that ID does not exist.", &lower_bound, &upper_bound);

  elem_t found_value;
  item_t *item = extract_item_from_storage(*storage, arr_names[id - 1], &found_value);

  puts("You have selected the following merchandise:");
  print_item(*item, id, true);

  while (true)
    {
      char *shelf_name = ask_question_shelf("Enter a storage location:\n\
(format: yxx, where y = letter 'A-Z', x = digit 0-9)");

      bool st_location_exists = location_exists(*storage, shelf_name, &found_value);
      bool names_equal = strcmp(found_value.s, get_item_name(*item)) == 0;

      if (st_location_exists && names_equal)
        {
          int tmp = 0;
          int *index = &tmp; 
          shelf_t *shelf = find_shelf_in_item_shelves(get_item_shelves(*item), shelf_name, index);
          ioopm_linked_list_remove(get_item_shelves(*item), *index);

          int lower_bound = 1;
          int stock_increase = ask_question_check_nr("Enter how much you want to increase the stock by:", "OBS! Minimum value is 1.", &lower_bound, NULL);
          
          increase_shelf_stock(shelf, stock_increase);
          
          add_shelf_to_item_shelves(get_item_shelves(*item), shelf);
          
          set_item_shelves(item, get_item_shelves(*item));
          add_item_to_storage(storage, item);
        }
      
      if (st_location_exists && !names_equal)
        {
          puts("OBS! The storage location you entered is already occupied by another item.");
          free(shelf_name);
          continue;
        }
      
      if (!st_location_exists)
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



static bool no_carts_in_storage(storage_t *storage)
{
  if (get_storage_carts_amount(*storage) == 0)
    {
      puts("OBS! No carts have been added to the warehouse yet.");
      return true;
    }
  else
    {
      return false;
    }
}



static void list_carts(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (no_carts_in_storage(storage)) return;
  
  int carts_in_storage = get_storage_carts_amount(*storage);
  int continues = 1;
  
  for (int i = 0; i < carts_in_storage; i++)
    {
      cart_t cart = *(cart_t*) ioopm_linked_list_get(storage->carts, i).v;

      print_cart(cart);

      bool twenty_listings = i == continues * 20 - 1 && carts_in_storage > continues * 20;
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



void create_cart(storage_t *storage)
{
  if (storage_null(storage)) return;
  
  add_cart_to_storage(storage);
  puts("Cart successfully added!");
}



void remove_cart(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (no_carts_in_storage(storage)) return;
  
  list_carts(storage);

  int cart_id = 0;
  while (true)
    {
      cart_id = ask_question_int("Enter the number id of the cart you would like to remove:");
      if (cart_exists(storage, cart_id))
        {
          break;
        }
      else
        {
          puts("OBS! Cart ID does not exist.");
        }
    }

  cart_t cart = *extract_cart_from_storage(storage, cart_id);
  while (true)
    {
      print_cart(cart);
      
      char *key = ask_question_yes_no(("Are you sure you want to remove the selected shopping cart? (y/n)"));
      char key_up = toupper(*key);
      if (key_up == 'Y')
        {
          remove_cart_from_storage(storage, cart_id);
          puts("Cart successfully removed!");
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



void add_to_cart(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  if (no_carts_in_storage(storage)) return;
  
  list_items(storage, false);

  int item_amount = get_storage_items_amount(*storage);
  char *arr_names[item_amount];
  item_names_to_sorted_array(*storage, arr_names);
  
  int lower_bound = 1;
  int upper_bound = item_amount;
  int item_id = ask_question_check_nr("Enter the number id of the item you would like to add:", "OBS! An item with that ID does not exist.", &lower_bound, &upper_bound);

  item_t item = *extract_item_from_storage(*storage, arr_names[item_id - 1], NULL);

  puts("You have selected the following item:");
  print_item(item, item_id, false);

  lower_bound = 0;
  upper_bound = get_item_stock(item);

  if (upper_bound == 0)
    {
      puts("OBS! The selected item is not in stock.");
      return;
    }
  
  printf("Current stock of %s is %d\n", get_item_name(item), upper_bound);
  int amount = ask_question_check_nr("Enter a quantity, or 0 to return to menu:", "OBS! You either entered a negative value, or a value greater than the current stock of the item.", &lower_bound, &upper_bound);

  if (amount == 0)
    {
      puts("Operation cancelled by user.");
      return;
    }
  
  while (true)
    {
      list_carts(storage);
      print_item(item, item_id, false);
      
      int cart_id = ask_question_int("Enter the number id of the cart you would like to add above item to:");
      
      if (cart_exists(storage, cart_id))
        {
          add_item_to_cart(storage, item, amount, cart_id);
          break;
        }
      else
        {
          puts("OBS! Cart ID does not exist.");
        }
    }
}



void remove_from_cart(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  if (no_carts_in_storage(storage)) return;

  cart_t *cart;
  
  while (true)
    {
      list_carts(storage);
      
      int cart_id = ask_question_int("Enter the number id of a non-empty cart you would like to remove an item from, or 0 to return to menu:");

      if (cart_id == 0)
        {
          puts("Operation cancelled by user.");
          return;
        }
      
      if (cart_exists(storage, cart_id))
        {
          cart = extract_cart_from_storage(storage, cart_id);
          if (get_cart_items_amount(*cart) == 0)
            {
              puts("OBS! This shopping cart is empty, please choose a non-empty one.");
              continue;
            }
          else
            {
              break;
            }
        }
      else
        {
          puts("OBS! Cart ID does not exist.");
        }
    }
  
  puts("You have selected the following shopping cart:");
  print_cart(*cart);

  int cart_items_amount = get_cart_items_amount(*cart);
  
  int lower_bound = 1;
  int upper_bound = cart_items_amount;
  int item_id = ask_question_check_nr("Enter the number id of the item you would like to remove from the shopping cart:", "OBS! An item with that ID does not exist in the selected shopping cart.", &lower_bound, &upper_bound);

  cart_item_t *cart_item = get_cart_item_from_cart(*cart, item_id);
  
  lower_bound = 0;
  upper_bound = get_cart_item_quantity(*cart_item);
  int amount = ask_question_check_nr("Enter a quantity, or 0 to return to menu:", "OBS! You either entered a negative value, or a value greater than the current quantity in the shopping cart.", &lower_bound, &upper_bound);

  if (amount == 0)
    {
      puts("Operation cancelled by user.");
      return;
    }

  if (upper_bound - amount == 0)
    {
      remove_item_from_cart(cart, item_id); 
    }
  else
    {
      increase_cart_item_quantity(cart_item, -amount);
    }
  
  puts("Item successfully removed from shopping cart!");
}



void calculate_cart_cost(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  if (no_carts_in_storage(storage)) return;

  cart_t *cart;
  
  while (true)
    {
      list_carts(storage);
      
      int cart_id = ask_question_int("Enter the number id of a non-empty cart you would like to calculate the cost of, or 0 to return to menu:");

      if (cart_id == 0)
        {
          puts("Operation cancelled by user.");
          return;
        }
      
      if (cart_exists(storage, cart_id))
        {
          cart = extract_cart_from_storage(storage, cart_id);
          if (get_cart_items_amount(*cart) == 0)
            {
              puts("OBS! This shopping cart is empty, please choose a non-empty one.");
              continue;
            }
          else
            {
              break;
            }
        }
      else
        {
          puts("OBS! Cart ID does not exist.");
        }
    }

  int total_cost = 0;
  for (int i = 0; i < get_cart_items_amount(*cart); i++)
    {
      cart_item_t cart_item = *get_cart_item_from_cart(*cart, i);
      item_t item = *extract_item_from_storage(*storage, get_cart_item_name(cart_item), NULL);
      total_cost += get_cart_item_quantity(cart_item) * (get_item_price(item) / 100);
    }
  print_cart(*cart);
  printf("Total cost of selected shopping cart is: %d kr\n", total_cost);
}



void cart_checkout(storage_t *storage)
{
  if (storage_null(storage)) return;
  if (storage_empty(storage)) return;
  if (no_carts_in_storage(storage)) return;  
  
  cart_t *cart;
  int cart_id = 0;
  
  while (true)
    {
      list_carts(storage);
      
      cart_id = ask_question_int("Enter the number id of a non-empty cart you would like to checkout, or 0 to return to menu:");
      
      if (cart_id == 0)
        {
          puts("Operation cancelled by user.");
          return;
        }
      
      if (cart_exists(storage, cart_id))
        {
          cart = extract_cart_from_storage(storage, cart_id);
          if (get_cart_items_amount(*cart) == 0)
            {
              puts("OBS! This shopping cart is empty, please choose a non-empty one.");
              continue;
            }
          else
            {
              break;
            }
        }
      else
        {
          puts("OBS! Cart ID does not exist.");
        }
    }

  for (int i = 0; i < get_cart_items_amount(*cart); i++)
    {
      cart_item_t *cart_item = get_cart_item_from_cart(*cart, i);
      deplete_stock(storage, cart_item);
    }
  remove_cart_from_storage(storage, cart_id);
  
  puts("Shopping cart successfully checked out!");
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
          add_item(storage);
        }
      if (key_up == 'L')
        {
          list_items(storage, false);
        }
      if (key_up == 'R')
        {
          remove_item(storage);
        }
      if (key_up == 'E')
        {
          edit_item(storage);
        }
      if (key_up == 'S')
        {
          show_item_stock(storage);
        }
      if (key_up == 'P')
        {
          replenish_item_stock(storage);
        }
      if (key_up == 'C')
        {
          create_cart(storage);
        }
      if (key_up == 'M')
        {
          remove_cart(storage);
        }
      if (key_up == 'D')
        {
          add_to_cart(storage);
        }
      if (key_up == 'O')
        {
          remove_from_cart(storage);
        }
      if (key_up == 'U')
        {
          calculate_cart_cost(storage);
        }
      if (key_up == 'K')
        {
          cart_checkout(storage);
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
