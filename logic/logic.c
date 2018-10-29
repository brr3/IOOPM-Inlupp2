#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "logic.h"
#include "elem.h"



//
// MEMORY MANAGEMENT
//

/// brief Free all memory allocated by a shelf from an item's shelves
/// param item  | The item to retrieve the shelves from
/// param index | The index of the shelf to destroy in the list of item shelves 
static void destroy_shelf(item_t *item, int index)
{
  ioopm_list_t *item_shelves = get_item_shelves(*item);
  shelf_t *shelf = ioopm_linked_list_get(item_shelves, index).v;
  free(get_shelf_name(*shelf));
  free(shelf);
  ioopm_linked_list_remove(item_shelves, index);
}

/// brief Free all memory allocated by an item's shelves
/// param item | The item to retrieve the shelves from
static void destroy_shelves(item_t *item)
{
  ioopm_list_t *item_shelves = get_item_shelves(*item);
  int locations = get_item_shelves_count(*item);
  for (int i = 0; i < locations; i++)
    {
      destroy_shelf(item, i);
    }
  ioopm_linked_list_destroy(item_shelves);  
}

/// brief Free all memory allocated by an item
/// param elem_key_ignored | Unused parameter
/// param elem_value       | An element containing an item to destroy
/// param x_ignored        | Unused parameter
static void destroy_item(elem_t elem_key_ignored, elem_t elem_value, void *x_ignored)
{
  item_t *item = elem_value.v;
  free(get_item_name(*item));
  free(get_item_desc(*item));
  destroy_shelves(item);  
  free(item);
}

/// brief Free all memory allocated by cart items in a cart
/// param cart | The cart to retrieve cart items from
static void destroy_cart_items(cart_t *cart)
{
  ioopm_list_t *cart_items = cart->cart_items;
  int cart_item_count = ioopm_linked_list_size(cart_items);
  for (int i = 0; i < cart_item_count; i++)
    {
      cart_item_t *cart_item = ioopm_linked_list_get(cart_items, i).v;
      free(cart_item);
    }
  ioopm_linked_list_destroy(cart_items);
}

/// brief Free all memory allocated by a cart
/// param elem_value | An element containing a cart to destroy
static void destroy_cart(elem_t elem_value, void *x_ignored)
{
  cart_t *cart = elem_value.v;
  destroy_cart_items(cart);
  free(cart);
}

void destroy_storage(storage_t *storage)
{
  ioopm_hash_table_apply_to_all(storage->items, destroy_item, NULL);          
  ioopm_hash_table_destroy(get_storage_items(*storage));
  ioopm_hash_table_destroy(get_storage_locations(*storage));
  ioopm_linked_apply_to_all(get_storage_carts(*storage), destroy_cart, NULL);
  ioopm_linked_list_destroy(get_storage_carts(*storage));
  free(storage);
}



//
// MENU LOGIC
//



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
  return isdigit(*(--str));
}



bool is_menu_key(char *key)
{
  switch(*key)
    {
    case 'A':
    case 'a':
    case 'L':
    case 'l':
    case 'R':
    case 'r':
    case 'E':
    case 'e':
    case 'S':
    case 's':
    case 'P':
    case 'p':
    case 'C':
    case 'c':
    case 'M':
    case 'm':
    case 'D':
    case 'd':
    case 'O':
    case 'o':
    case 'U':
    case 'u':
    case 'K':
    case 'k':
    case 'Q':
    case 'q':
      return true;
    default:
      return false;
    }
}



bool is_yn_key(char *key)
{
  switch(*key)
    {
    case 'Y':
    case 'y':
    case 'N':
    case 'n':
      return true;
    default:
      return false;
    }
}



//
// STORAGE LOGIC
//



/// brief A hash function that operates on strings
/// param key | An element containing a string
/// returns   | The hash value of key
static int hash_string(elem_t key)
{
  char *str = key.s;
  int result = 0;
  do
    {
      result = result * 31 + *str;
    }
  while (*++str != '\0'); 
  return result;
}

/// brief Check if two strings encapsulated in elem_t are equal
/// param a | An element containing a string
/// param b | An element containing a string
/// returns | True if the strings are equal, else false
static bool string_equal(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
}

storage_t *make_storage()
{
  storage_t *storage = calloc(1, sizeof(storage_t));
  set_storage_items(storage, ioopm_hash_table_create_custom(hash_string, string_equal, 17, 0.9));
  set_storage_locations(storage, ioopm_hash_table_create_custom(hash_string, string_equal, 17, 0.9));
  set_storage_carts(storage, ioopm_linked_list_create());
  set_storage_cart_counter(storage, 0);
  return storage;
}



//
// ITEM LOGIC
//



item_t *make_item(char *name, char *desc, int price) 
{
  item_t *item = calloc(1, sizeof(item_t));
  set_item_name(item, name);
  set_item_desc(item, desc);
  set_item_price(item, price);
  set_item_shelves(item, ioopm_linked_list_create());
  return item;
}



static char *to_upper(char *str)
{
   int c = 0;  
   while (str[c] != '\0')
     {
       if (str[c] >= 'a' && str[c] <= 'z')
         {
           str[c] = str[c] - 32;
         }
       c++;
     }
   return str;
}



bool item_exists(storage_t storage, char *item_name)
{
  elem_t found_element;
  elem_t elem_name = {.s = to_upper(item_name)};
  return ioopm_hash_table_lookup(get_storage_items(storage), elem_name, &found_element);
}



void remake_item(storage_t *storage, item_t *item)
{
  item_t *new_item = calloc(1, sizeof(item_t));
  set_item_name(new_item, get_item_name(*item));
  set_item_desc(new_item, get_item_desc(*item));
  set_item_price(new_item, get_item_price(*item));
  set_item_shelves(new_item, get_item_shelves(*item));

  add_item_to_storage(storage, new_item);

  free(item); 
}



void add_item_to_storage(storage_t *storage, item_t *item)
{
  elem_t elem_key = {.s = to_upper(get_item_name(*item))};
  elem_t elem_value = {.v = item};
  ioopm_hash_table_insert(get_storage_items(*storage), elem_key, elem_value);
}



void remove_item_from_storage(storage_t *storage, item_t *item)
{
  for (int i = 0; i < get_item_shelves_count(*item); i++)
    {
      shelf_t *shelf = ioopm_linked_list_get(get_item_shelves(*item), i).v;
      elem_t elem_key_to_remove = {.s = get_shelf_name(*shelf)};
      ioopm_hash_table_remove_entry(get_storage_locations(*storage), elem_key_to_remove);
    } 
  elem_t elem_key_to_remove = {.s = get_item_name(*item)};
  ioopm_hash_table_remove_entry(get_storage_items(*storage), elem_key_to_remove);
  elem_t elem_value_to_remove = {.v = item};
  destroy_item((elem_t) {.v = NULL}, elem_value_to_remove, NULL);  
}



/// brief Check if two string pointers are equal
/// param p1 | A pointer to a string
/// param p2 | A pointer to a string
/// returns  | True if the strings are not equal, else false
static int cmp_string_ptr(const void *p1, const void *p2)
{
  return strcmp(*(char *const *) p1, *(char *const *) p2);
}

/// brief Sort keys alphabetically
/// param keys    | Array to sort
/// param no_keys | Amount of elements in keys array
static void sort_keys(char *keys[], size_t no_keys)
{  
  qsort(keys, no_keys, sizeof(char *), cmp_string_ptr);
}

void item_names_to_sorted_array(storage_t storage, char *arr_names[])
{
  int item_count = get_storage_items_amount(storage);
  ioopm_list_t *item_names = ioopm_hash_table_keys(get_storage_items(storage));
  for (int i = 0; i < item_count; i++)
    {
      arr_names[i] = ioopm_linked_list_remove(item_names, i).s;
    }
  sort_keys(arr_names, item_count);
  ioopm_linked_list_destroy(item_names);
}



item_t *extract_item_from_storage(storage_t storage, char *item_name, elem_t *found_value)
{
  if (!found_value)
    {
      elem_t found_value;
      elem_t elem_key = {.s = item_name};
      ioopm_hash_table_lookup(get_storage_items(storage), elem_key, &found_value);
      item_t *item = found_value.v;
      if (item == NULL)
        {
          puts("ERROR: NULL pointer dereference.");
          assert(false); 
        }
      return item;
    }
  else
    {
      elem_t elem_key = {.s = item_name};
      ioopm_hash_table_lookup(get_storage_items(storage), elem_key, found_value); // Side effect
      item_t *item = found_value->v;
      if (item == NULL)
        {
          puts("ERROR: NULL pointer dereference.");
          assert(false); 
        }
      return item;
    }
}



//
// SHELF LOGIC
//



/// brief Create a shelf with the given parameters
/// param name   | Name of the shelf
/// param amount | Amount of merchandise to be stored on the shelf
/// return       | A newly created shelf
static shelf_t *make_shelf(char *name, int amount)
{
  shelf_t *shelf = calloc(1, sizeof(shelf_t));
  set_shelf_name(shelf, name);
  set_shelf_stock(shelf, amount);
  return shelf;
}



bool location_exists(storage_t storage, char *shelf_name, elem_t *found_value)
{
  if (!found_value)
    {
      elem_t found_value;
      elem_t elem_shelf_name = {.s = shelf_name};
      return ioopm_hash_table_lookup(get_storage_locations(storage), elem_shelf_name, &found_value);
    }
  else
    {
      elem_t elem_shelf_name = {.s = shelf_name};
      return ioopm_hash_table_lookup(get_storage_locations(storage), elem_shelf_name, found_value);
    }
}



/// brief Add a shelf to an item
/// param item  | Item to add shelf to
/// param shelf | Shelf to add
static void add_shelf_to_item(item_t *item, shelf_t *shelf)
{
  elem_t elem_shelf = {.v = shelf};          
  ioopm_linked_list_prepend(get_item_shelves(*item), elem_shelf);
}



/// brief Insert the name of an item into the second hash table mentioned below
/// param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// param shelf   | Shelf to extract a name from to use as key
/// param item    | Item to extract a name from to be inserted in the hash table
static void add_shelf_to_locations(storage_t *storage, shelf_t shelf, item_t item)
{
  elem_t elem_key = {.s = get_shelf_name(shelf)};
  elem_t elem_value = {.s = get_item_name(item)};
  ioopm_hash_table_insert(get_storage_locations(*storage), elem_key, elem_value);
}



shelf_t *find_shelf_in_item(item_t *item, char *shelf_name_to_find)
{
  int shelves_count = ioopm_linked_list_size(get_item_shelves(*item));
  shelf_t *shelf;
  for (int i = 0; i < shelves_count; i++)
    {
      shelf = ioopm_linked_list_get(get_item_shelves(*item), i).v;
      if (strcmp(get_shelf_name(*shelf), shelf_name_to_find) == 0)
        {
          break;
        }
    }
  return shelf;
}



void add_shelf_to_storage(storage_t *storage, item_t *item, char *shelf_name, int stock)
{
  shelf_t *shelf = make_shelf(shelf_name, stock);
  add_shelf_to_item(item, shelf);        
  add_shelf_to_locations(storage, *shelf, *item);
}



/// brief Deplete the stock of an item in storage
/// param storage | A storage containing two hash tables, one that maps names of items to
///               | ...the address of the full information for that item, one that maps names
///               | ...of storage locations to the name of the item stored at that location,
///               | ...and a linked list of shopping carts
static void deplete_stock(storage_t *storage, cart_item_t *cart_item)
{
  item_t *item = extract_item_from_storage(*storage, get_cart_item_name(*cart_item), NULL);

  int shelves_count = get_item_shelves_count(*item);
  int total_stock_decrease = get_cart_item_quantity(*cart_item);
  int depleted_stock = 0;

  ioopm_list_t *item_shelves = get_item_shelves(*item);

  for (int i = 0; i < shelves_count; i++)
    {
      shelf_t *shelf = ioopm_linked_list_get(item_shelves, i).v;
      while (get_shelf_stock(*shelf) > 0)
        {
          set_shelf_stock(shelf, get_shelf_stock(*shelf) - 1);
          ++depleted_stock;
          if (get_shelf_stock(*shelf) == 0) break;
          if (depleted_stock >= total_stock_decrease) return;
        }
      elem_t elem_shelf_name = {.s = get_shelf_name(*shelf)};
      ioopm_hash_table_remove_entry(get_storage_locations(*storage), elem_shelf_name);
      destroy_shelf(item, i);
    }
}

void checkout_cart_items(storage_t *storage, cart_t *cart)
{
  int cart_id = get_cart_id(*cart);
  for (int i = 0; i < get_cart_items_amount(*cart); i++)
    {
      cart_item_t *cart_item = get_cart_item_from_cart(*cart, i);
      deplete_stock(storage, cart_item);
    }
  remove_cart_from_storage(storage, cart_id);
}



//
// CART LOGIC
//



/// brief Create a new shopping cart with the given parameter
/// param cart_id | A unique identifier for the shopping cart that increases monotonically for
///               | ...each new shopping cart that is created
/// returns       | A newly created shopping cart
static cart_t *make_cart(int id)
{
  cart_t *cart = calloc(1, sizeof(cart_t));
  set_cart_id(cart, id);
  set_cart_items(cart, ioopm_linked_list_create());
  return cart;
}



/// brief Convert a shopping cart id to the corresponding index in the list of shopping carts
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// param cart_id  | The shopping cart id to convert 
static int cart_id_to_index(storage_t storage, int cart_id)
{
  int carts_in_storage = get_storage_carts_amount(storage); 
  for (int i = 0; i < carts_in_storage; i++)
    {
      cart_t cart = *(cart_t*) ioopm_linked_list_get(get_storage_carts(storage), i).v;
      if (cart.id == cart_id)
        {
          return i;
        }
    }
  return -1;
}



bool cart_exists(storage_t storage, int cart_id)
{
  int valid_index = cart_id_to_index(storage, cart_id);
  return valid_index != -1;
}



void add_cart_to_storage(storage_t *storage)
{
  cart_t *cart = make_cart(increase_cart_counter(storage));
  elem_t elem_cart = {.v = cart};
  ioopm_linked_list_append(get_storage_carts(*storage), elem_cart);
}



void remove_cart_from_storage(storage_t *storage, int cart_id)
{
  int valid_index = cart_id_to_index(*storage, cart_id);
  cart_t *cart = ioopm_linked_list_get(get_storage_carts(*storage), valid_index).v;
  if (valid_index >= 0)
    {
      ioopm_linked_list_remove(get_storage_carts(*storage), valid_index);
    }
  elem_t elem_cart_to_remove = {.v = cart};
  destroy_cart(elem_cart_to_remove, NULL);
}



cart_t *extract_cart_from_storage(storage_t storage, int cart_id)
{
  int valid_index = cart_id_to_index(storage, cart_id);
  if (valid_index >= 0)
    {
      cart_t *cart = ioopm_linked_list_get(get_storage_carts(storage), valid_index).v;
      return cart;
    }
  else
    {
      return NULL;
    }
}



/// brief Create a new cart item with the given parameters
/// param name     | Name of the item
/// param quantity | Quantity of item to add to the shopping cart
/// returns        | A newly created cart item
static cart_item_t *make_cart_item(char *name, int quantity)
{
  cart_item_t *cart_item = calloc(1, sizeof(cart_item_t));
  set_cart_item_name(cart_item, name);
  set_cart_item_quantity(cart_item, quantity);
  return cart_item;
}

/// brief Check if a cart contains an item
/// param cart      | The cart to search
/// param item_name | The name of item to find
/// returns         | True if cart contains item, else false
static bool cart_item_exists(cart_t cart, char *item_name)
{
  ioopm_list_t *cart_items = get_cart_items(cart);
  int cart_items_count = get_cart_items_amount(cart);
  for (int i = 0; i < cart_items_count; i++)
    {
      cart_item_t *cart_item = ioopm_linked_list_get(cart_items, i).v;
      if (strcmp(cart_item->name, item_name) == 0)
        {
          return true;
        }
    }
  return false;
}

void add_item_to_cart(storage_t storage, item_t item, int amount, int cart_id)
{
  cart_t *cart = extract_cart_from_storage(storage, cart_id);
  if (cart != NULL)
    {
      if (cart_item_exists(*cart, get_item_name(item)))
        {
          int valid_index = cart_id_to_index(storage, cart_id);
          cart_item_t *cart_item = ioopm_linked_list_get(get_cart_items(*cart), valid_index).v;
          if (get_cart_item_quantity(*cart_item) + amount <= get_item_stock(item))
            {
              increase_cart_item_quantity(cart_item, amount);
              puts("Item successfully added to shopping cart!");
            }
          else
            {
              puts("OBS! Amount of selected item in shopping cart exceeds stock. Operation cancelled.");
            }
        }
      else
        {         
          cart_item_t *cart_item = make_cart_item(get_item_name(item), amount);
          elem_t elem_cart_item = {.v = cart_item};
          ioopm_linked_list_append(get_cart_items(*cart), elem_cart_item);
          
          puts("Item sucessfully added to shopping cart!");
        }
    }
  else
    {
      puts("ERROR: NULL pointer dereference!");
    }
}



void remove_item_from_cart(cart_t *cart, int item_id)
{
  ioopm_list_t *cart_items = get_cart_items(*cart);
  cart_item_t *cart_item = ioopm_linked_list_get(cart_items, item_id - 1).v;
  ioopm_linked_list_remove(cart_items, item_id - 1);
  free(cart_item);
}



//
// PRINT FUNCTIONS
//



void print_item(item_t item, int id, bool print_stock)
{
  int kr  = get_item_price(item) / 100;
  int ore = get_item_price(item) % 100;
  char *name = get_item_name(item);
  char *desc = get_item_desc(item);

  if (print_stock) // Show stock
    {
      int shelves_count = get_item_shelves_count(item);      
      if (shelves_count == 0)
        {
          printf("--------[ # %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d\nShelves: None\n", name, desc, kr, ore);
          puts("------------------------");
          return;
        }      
      else if (shelves_count == 1)
        {
          ioopm_list_t *item_shelves = get_item_shelves(item);
          shelf_t shelf = *(shelf_t*) ioopm_linked_list_get(item_shelves, 0).v;
          int item_amount = get_item_stock(item);
          printf("--------[ # %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelf: %s\nAmount: %d\n", name, desc, kr, ore, shelf.shelf_name, item_amount);
          puts("------------------------");
        }
      else
        {
          ioopm_list_t *item_shelves = get_item_shelves(item);          
          int item_amount = get_item_stock(item);
          printf("--------[ # %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves:\n", name, desc, kr, ore);
          for (int i = 0; i < (int) shelves_count; i++) // Optional: sort storage locations
            {
              shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(item_shelves, i).v); 
              if (i < shelves_count - 1)
                {
                  printf("(%s, %d), ", get_shelf_name(shelf), get_shelf_stock(shelf)); 
                }
              else
                {
                  printf("(%s, %d)\n", get_shelf_name(shelf), get_shelf_stock(shelf));
                }
            }
          printf("Total amount: %d\n", item_amount);
          puts("------------------------");
        }
    }
  else
    {
      printf("--------[ # %d ]--------\n", id);
      printf("Name: %s\nDesc: %s\nPrice: %d.%d\n", name, desc, kr, ore); // List merchandise
      puts("------------------------");
      return;
    }
}



void print_cart(cart_t cart)
{
  int id = get_cart_id(cart);
  ioopm_list_t *cart_items = get_cart_items(cart);

  int items_in_cart = get_cart_items_amount(cart);

  if (items_in_cart == 0)
    {
      printf("--------[ # %d ]--------\n", id);
      puts("Cart empty!");
      puts("------------------------");
    }
  else if (items_in_cart == 1)
    {
      cart_item_t cart_item = *(cart_item_t*) ioopm_linked_list_get(cart_items, 0).v;
      printf("--------[ # %d ]--------\n", id);
      printf("1. Merchandise: %s\tAmount: %d\n", cart_item.name, cart_item.quantity);
      puts("------------------------");
    }
  else
    {
      printf("--------[ # %d ]--------\n", id);
      for (int i = 0; i < items_in_cart; i++)
        {
          cart_item_t cart_item = *(cart_item_t*) ioopm_linked_list_get(cart_items, i).v;
          printf("%d. Merchandise: %s\tAmount: %d\n", i + 1, cart_item.name, cart_item.quantity);
        }
      puts("------------------------");
    }
}
