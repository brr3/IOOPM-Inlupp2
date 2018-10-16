#include <stdio.h>
#include <stdbool.h>
#include "logic.h"
#include "elem.h"


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

void sort_keys(char *keys[], size_t no_keys)
{  
  qsort(keys, no_keys, sizeof(char *), cmp_string_ptr);
}

void ht_names_to_sorted_array(ioopm_hash_table_t *items, char *arr_names[])
{
  int merch_count = ioopm_hash_table_size(items);
  ioopm_list_t *merch_names = ioopm_hash_table_keys(items);
  for (int i = 0; i < merch_count; i++)
    {
      arr_names[i] = ioopm_linked_list_remove(merch_names, i).s; // Faster than linked_list_get
    }
  sort_keys(arr_names, merch_count);
  ioopm_linked_list_destroy(merch_names);
}

void print_item(item_t item, int id, bool print_stock)
{
  int kr  = get_item_price(item) / 100;
  int ore = get_item_price(item) % 100;
  char *name = get_item_name(item);
  char *desc = get_item_desc(item);

  if (print_stock) // Show stock
    {
      size_t shelves_count = get_item_shelves_count(item);      
      if (shelves_count == 0)
        {
          printf("--------[ %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d\nShelves: None\n", name, desc, kr, ore);
          return;
        }
      ioopm_list_t *shelves = get_item_shelves(item);
      shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(shelves, 0).v);
      int item_amount = get_item_total_amount(item);
      
      if (shelves_count == 1)
        {
          printf("--------[ %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: %s\nAmount: %d\n", name, desc, kr, ore, shelf.shelf_name, item_amount);
        }
      
      if (shelves_count > 1)
        {
          printf("--------[ %d ]--------\n", id);
          printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: ", name, desc, kr, ore);
          for (int i = 0; i < (int) shelves_count; i++)
            {
              shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(shelves, i).v); 
              if (i < (int) shelves_count - 1)
                {
                  printf("%s, ", shelf.shelf_name); 
                }
              else
                {
                  printf("%s\n", shelf.shelf_name);
                }
            }
          printf("Amount: %d\n", item_amount);
        }
    }
  else
    {
      printf("--------[ %d ]--------\n", id);
      printf("Name: %s\nDesc: %s\nPrice: %d.%d\n", name, desc, kr, ore); // List merchandise
      return;
    }
}
