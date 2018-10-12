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

bool cmp_int(elem_t a, elem_t b)
{
  return a.i - b.i == 0;
}


item_t make_merch(char *name, char *desc, int price, char *shelf_name) 
{
  item_t item = {};
  set_item_name(&item, name);
  set_item_desc(&item, desc);
  set_item_price(&item, price);
  initiate_item_shelves(&item, shelf_name);
  return item;
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
    case 'L':
    case 'l':
    case 'T':
    case 't':
    case 'R':
    case 'r':
    case 'G':
    case 'g':
    case 'H':
    case 'h':
    case 'A':
    case 'a':
      return true;
    default:
      return false;
    }
}


void print_item(item_t item)
{
  int kr  = get_item_price(item) / 100;
  int ore = get_item_price(item) % 100;
  char *name = get_item_name(item);
  char *desc = get_item_desc(item);
  ioopm_list_t *shelves = get_item_shelves(item);
  size_t shelves_count = get_item_shelves_count(item);

  shelf_t *shelf = ioopm_linked_list_get(shelves, 0).v;
  if (shelves_count == 1)
    {
      printf("--------------------- \n");
      printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: %s\nAmount: %d\n", name, desc, kr, ore, shelf->shelf_name, shelf->amount);
    }
  else
    {
      printf("--------------------- \n");
      printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: ", name, desc, kr, ore);
      for (int i = 0; i < (int) shelves_count; i++)
        {
          shelf_t *shelf = ioopm_linked_list_get(shelves, i).v;
          if (i < (int) shelves_count - 1)
            {
              printf("%s, ", shelf->shelf_name); 
            }
          else
            {
              printf("%s\n", shelf->shelf_name);
            }
        }
      printf("Amount: %d\n", get_item_total_amount(item));
    }
}


void list_db(item_t *items, int no_items) 
{
  char *name = items->name;

  for (int i = 0; i < no_items; i++)
  {
    name = items[i].name;
    printf("%d. %s\n", i + 1, name);
  }
}


item_t input_merch(void) 
{
  char *name = ask_question_string("Enter a name: ");
  char *desc = ask_question_string("Enter a description: ");
  int price = ask_question_int("Enter a price: ");
  char *shelf = ask_question_shelf("Enter a shelf: ");
  
  return make_merch(name, desc, price, shelf);
}


item_t edit_db(item_t *items, int no_items)
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
}


void add_merch_to_db(ioopm_hash_table_t *items)
{
  item_t item = input_merch();
  elem_t elem_name = {.s = item.name};
  elem_t elem_item = {.v = &item};
  ioopm_hash_table_insert(items, elem_name, elem_item);
}


item_t remove_item_from_db(item_t *items, int no_items)
{ 
  printf("--------------------- \n");
  list_db(items, no_items);

  while (true)
    {
      int editpos = ask_question_int("Enter the number of the item you want to remove: ");

      if (editpos < 1 || editpos > no_items)
        {
          printf("OBS! Enter a number between 1 and %d.\n", no_items); 
        } else
        {
          printf("--------------------- \n");
          printf("The following item has now been removed: \n");
          print_item(items[editpos - 1]);         

          for (int i = editpos - 1; i <= no_items - 1; i++)
            {
              items[i] = items[i+1];
            }
          return *items;
        }
    }  
}
