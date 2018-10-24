#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "logic.h"
#include "elem.h"



static void free_hash_table_keys_values(elem_t elem_key_ignored, elem_t elem_value, void *x_ignored)
{
  item_t *item = (item_t*) elem_value.v;
  free(get_item_name(*item));
  free(get_item_desc(*item));
  ioopm_list_t *item_shelves = get_item_shelves(*item);
  int locations = get_item_shelves_count(*item);
  for (int i = 0; i < locations; i++)
    {
      shelf_t *shelf = ioopm_linked_list_get(item_shelves, i).v;
      free(get_shelf_name(*shelf));
      free(shelf);
    }
  ioopm_linked_list_destroy(item_shelves);
  free(elem_value.v);
}

void destroy_storage(storage_t *storage)
{
  ioopm_hash_table_apply_to_all(storage->items, free_hash_table_keys_values, NULL);          
  ioopm_hash_table_destroy(storage->items);
  ioopm_hash_table_destroy(storage->locations);
  free(storage);
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
  return isdigit(*str);
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



bool merch_exists(storage_t *storage, char *merch_name)
{
  elem_t found_element;
  elem_t elem_name = {.s = to_upper(merch_name)};
  return ioopm_hash_table_lookup(storage->items, elem_name, &found_element);
}



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

static bool cmp_string(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
}

storage_t *make_storage()
{
  ioopm_hash_table_t *items = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  ioopm_hash_table_t *locations = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  storage_t *storage = calloc(1, sizeof(storage_t));
  storage->items = items;
  storage->locations = locations;
  return storage;
}



item_t *make_merch(char *name, char *desc, int price) 
{
  item_t *item = calloc(1, sizeof(item_t));
  set_item_name(item, name);
  set_item_desc(item, desc);
  set_item_price(item, price);
  ioopm_list_t *shelves = ioopm_linked_list_create();
  item->shelves = shelves;
  return item;
}



void remake_merch(storage_t *storage, item_t *old_item)
{
  item_t *new_item = calloc(1, sizeof(item_t)); // Side effect
  set_item_name(new_item, get_item_name(*old_item));
  set_item_desc(new_item, get_item_desc(*old_item));
  set_item_price(new_item, get_item_price(*old_item));
  set_item_shelves(new_item, get_item_shelves(*old_item));
  
  add_item_to_storage(storage, new_item); // Side effect

  free(old_item); // Side effect
}



shelf_t *make_shelf(char *shelf_name, int amount)
{
  shelf_t *shelf = calloc(1, sizeof(shelf_t));
  shelf->shelf_name = shelf_name;
  shelf->amount = amount;
  return shelf;
}



shelf_t *find_shelf_in_item_shelves(ioopm_list_t *item_shelves, char *shelf_name, int *index)
{
  int shelves_count = ioopm_linked_list_size(item_shelves);
  shelf_t *shelf;
  for (int i = 0; i < shelves_count; i++)
    {
      shelf = (shelf_t*) ioopm_linked_list_get(item_shelves, i).v;
      if (strcmp(shelf->shelf_name, shelf_name) == 0)
        {
          *index = i; // Side effect
          break;
        }
    }
  return shelf;
}



void add_item_to_storage(storage_t *storage, item_t *item)
{
  elem_t elem_key = {.s = to_upper(get_item_name(*item))};
  elem_t elem_value = {.v = item};
  ioopm_hash_table_insert(storage->items, elem_key, elem_value); // Side effect
}



void remove_item_from_storage(storage_t *storage, item_t *item)
{
  for (int i = 0; i < get_item_shelves_count(*item); i++)
    {
      shelf_t *shelf = (shelf_t*) ioopm_linked_list_get(get_item_shelves(*item), i).v;
      elem_t elem_key_to_remove = {.s = get_shelf_name(*shelf)};
      ioopm_hash_table_remove_entry(storage->locations, elem_key_to_remove); // Side effect
    } 
  elem_t elem_key_to_remove = {.s = get_item_name(*item)};
  ioopm_hash_table_remove_entry(storage->items, elem_key_to_remove); // Side effect
}



static int cmp_string_ptr(const void *p1, const void *p2)
{
  return strcmp(*(char *const *) p1, *(char *const *) p2);
}

static void sort_keys(char *keys[], size_t no_keys)
{  
  qsort(keys, no_keys, sizeof(char *), cmp_string_ptr);
}

void storage_names_to_sorted_array(storage_t *storage, char *arr_names[])
{
  int merch_count = ioopm_hash_table_size(storage->items);
  ioopm_list_t *merch_names = ioopm_hash_table_keys(storage->items);
  for (int i = 0; i < merch_count; i++)
    {
      arr_names[i] = ioopm_linked_list_remove(merch_names, i).s; // Side effect
    }
  sort_keys(arr_names, merch_count); // Side effect
  ioopm_linked_list_destroy(merch_names); // Side effect
}



item_t *extract_item_from_storage(storage_t *storage, char *item_name, elem_t *found_value)
{
  if (found_value == NULL)
    {
      elem_t found_value;
      elem_t elem_key = {.s = item_name};
      ioopm_hash_table_lookup(storage->items, elem_key, &found_value);
      item_t *item = (item_t*) found_value.v;
      if (item == NULL)
        {
          assert(false); // Defensiv programmering I22
        }
      return item;
    }
  else
    {
      elem_t elem_key = {.s = item_name};
      ioopm_hash_table_lookup(storage->items, elem_key, found_value); // Side effect
      item_t *item = (item_t*) found_value->v;
      if (item == NULL)
        {
          assert(false); // Defensiv programmering I22
        }
      return item;
    }
}



void add_shelf_to_item_shelves(ioopm_list_t *item_shelves, shelf_t *shelf)
{
  elem_t elem_shelf = {.v = shelf};          
  ioopm_linked_list_prepend(item_shelves, elem_shelf); // Side effect
}



void add_shelf_to_locations(storage_t *storage, char *shelf_name, char *item_name)
{
  elem_t elem_key = {.s = shelf_name};
  elem_t elem_value = {.s = item_name};
  ioopm_hash_table_insert(storage->locations, elem_key, elem_value); // Side effect
}



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
      ioopm_list_t *item_shelves = get_item_shelves(item);
      shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(item_shelves, 0).v);
      int item_amount = get_item_total_amount(item);
      
      if (shelves_count == 1)
        {
          printf("--------[ # %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelf: %s\nAmount: %d\n", name, desc, kr, ore, shelf.shelf_name, item_amount);
          puts("------------------------");
        }
      
      if (shelves_count > 1)
        {
          printf("--------[ # %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves:\n", name, desc, kr, ore);
          for (int i = 0; i < (int) shelves_count; i++) // Optional: sort storage locations
            {
              shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(item_shelves, i).v); 
              if (i < shelves_count - 1)
                {
                  printf("(%s, %d), ", get_shelf_name(shelf), get_shelf_amount(shelf)); 
                }
              else
                {
                  printf("(%s, %d)\n", get_shelf_name(shelf), get_shelf_amount(shelf));
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
