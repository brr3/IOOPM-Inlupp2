#include <stdio.h>
#include <stdbool.h>
#include "logic.h"
#include "elem.h"


void free_hash_table_keys_values(elem_t key_ignored, elem_t value, void *x_ignored)
{
  free(get_item_name(*(item_t*) value.v));
  free(get_item_desc(*(item_t*) value.v));
  ioopm_list_t *item_locations = get_item_shelves((*(item_t*) value.v));
  int locations = ioopm_linked_list_size(item_locations);
  for (int i = 0; i < locations; i++)
    {
      shelf_t *shelf = ioopm_linked_list_get(item_locations, i).v;
      free(get_shelf_name(*shelf));
      free(shelf);
    }
  ioopm_linked_list_destroy(item_locations);
  free(value.v);
}


int hash_string(elem_t key)
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


bool cmp_string(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
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


bool no_merch(int merch_count)
{
  if (merch_count == 0)
    {
      puts("OBS! No merchandise has been added to the warehouse yet.");
      return true;
    }
  else
    {
      return false;
    }
}


bool merch_exists(ioopm_hash_table_t *items, char *name)
{
  elem_t found_element;
  elem_t elem_name = {.s = name};
  return ioopm_hash_table_lookup(items, elem_name, &found_element);
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


void remake_merch(ioopm_hash_table_t *items, item_t *old_item)
{           
  item_t *new_item = calloc(1, sizeof(item_t)); // Side effect
  set_item_name(new_item, get_item_name(*old_item));
  set_item_desc(new_item, get_item_desc(*old_item));
  set_item_price(new_item, get_item_price(*old_item));
  set_item_shelves(new_item, get_item_shelves(*old_item));

  add_item_to_storage(items, new_item); // Side effect

  free(old_item); // Side effect
}


shelf_t *make_shelf(char *shelf_name, int amount)
{
  shelf_t *shelf = calloc(1, sizeof(shelf_t));
  shelf->shelf_name = shelf_name;
  shelf->amount = amount;
  return shelf;
}


shelf_t *find_shelf_in_list(ioopm_list_t *item_locations, char *shelf_name, int *index)
{
  int shelves_count = ioopm_linked_list_size(item_locations);
  shelf_t *shelf;
  for (int i = 0; i < shelves_count; i++)
    {
      shelf = (shelf_t*) ioopm_linked_list_get(item_locations, i).v;
      if (strcmp(shelf->shelf_name, shelf_name) == 0)
        {
          *index = i; // Side effect
          break;
        }
    }
  return shelf;
}


char *to_upper(char* str) // Not working
{
  while (*str)
    {
      *str = toupper((unsigned char) *str);
      str++;
    }
  return str;
}


static int cmp_string_ptr(const void *p1, const void *p2)
{
  return strcmp(*(char *const *) p1, *(char *const *) p2);
}


static void sort_keys(char *keys[], size_t no_keys)
{  
  qsort(keys, no_keys, sizeof(char *), cmp_string_ptr);
}


void add_item_to_storage(ioopm_hash_table_t *items, item_t *item)
{
  elem_t elem_key = {.s = get_item_name(*item)};
  elem_t elem_value = {.v = item};
  ioopm_hash_table_insert(items, elem_key, elem_value); // Side effect
}


void remove_item_from_storage(ioopm_hash_table_t *items, item_t *item)
{
  elem_t key_to_remove = {.s = get_item_name(*item)};
  ioopm_hash_table_remove_entry(items, key_to_remove);
}


void storage_names_to_sorted_array(ioopm_hash_table_t *items, char *arr_names[])
{
  int merch_count = ioopm_hash_table_size(items);
  ioopm_list_t *merch_names = ioopm_hash_table_keys(items);
  for (int i = 0; i < merch_count; i++)
    {
      arr_names[i] = ioopm_linked_list_remove(merch_names, i).s; // Side effect
    }
  sort_keys(arr_names, merch_count); // Side effect
  ioopm_linked_list_destroy(merch_names);
}


item_t *extract_item_from_storage(ioopm_hash_table_t *items, int i, char *arr_names[], elem_t *found_value)
{
  if (found_value == NULL)
    {
      elem_t found_value;
      elem_t elem_key = {.s = arr_names[i]};
      ioopm_hash_table_lookup(items, elem_key, &found_value);
      item_t *item = (item_t*) found_value.v;
      return item;
    }
  else
    {
      elem_t elem_key = {.s = arr_names[i]};
      ioopm_hash_table_lookup(items, elem_key, found_value); // Side effect
      item_t *item = (item_t*) found_value->v;
      return item;
    }
}


void add_shelf_to_list(ioopm_list_t *item_locations, shelf_t *shelf)
{
  elem_t elem_shelf = {.v = shelf};          
  ioopm_linked_list_prepend(item_locations, elem_shelf); // Side effect
}


void add_shelf_to_ht(ioopm_hash_table_t *locations, shelf_t *shelf)
{
  elem_t elem_key = {.s = get_shelf_name(*shelf)};
  elem_t elem_value = {.v = shelf};
  ioopm_hash_table_insert(locations, elem_key, elem_value); // Side effect
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
      ioopm_list_t *shelves = get_item_shelves(item);
      shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(shelves, 0).v);
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
              shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(shelves, i).v); 
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
