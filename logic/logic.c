#include <stdio.h>
#include <stdbool.h>
#include "logic.h"
#include "../utils/utils.h"


char *get_item_name(item_t item)
{
  return item.name;
}

char *get_item_desc(item_t item)
{
  return item.desc;
}

int get_item_price(item_t item)
{
  return item.price;
}

ioopm_list_t *get_item_shelves(item_t item)
{
  return ioopm_hash_table_keys(item.shelves);
}

int get_item_amount(item_t item)
{
  return item.amount;
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


bool cmp_int(elem_t a, elem_t b)
{
  return a.i - b.i == 0;
}


item_t make_merch(char *name, char *desc, int price, char *shelf)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create_custom(hash_string, cmp_int, 17, 0.9);
  elem_t elem_shelf = {.s = shelf};
  elem_t elem_amount = {.i = 0};
  ioopm_hash_table_insert(ht, elem_shelf, elem_amount);
  item_t t = { .name = name, .desc = desc, .price = price, .shelves = ht, .amount = 0};
  return t;
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
  if (!isdigit(*str))
    {
      return false;
    }
  return true;
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


void print_item(item_t i) // rätt fil??
{
  int kr  = get_item_price(i) / 100;
  int ore = get_item_price(i) % 100;
  char *name = get_item_name(i);
  char *desc = get_item_desc(i);
  ioopm_list_t *shelves = get_item_shelves(i);
  size_t list_size = ioopm_linked_list_size(shelves);
  int amount = get_item_amount(i);

  if (list_size == 1)
    {
      printf("--------------------- \n");
      printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: %s\nAmount: %d\n", name, desc, kr, ore, ioopm_linked_list_get(shelves, 0).s, amount);
    }
  else
    {
      printf("--------------------- \n");
      printf("Name: %s\nDesc: %s\nPrice: %d.%d kr\nShelves: ", name, desc, kr, ore);
      for (int i = 0; i < (int) list_size; i++)
        {
          if (i < (int) list_size - 1)
            {
              printf("%s, ", ioopm_linked_list_get(shelves, i).s); 
            }
          else
            {
              printf("%s\n", ioopm_linked_list_get(shelves, i).s);
            }
        }
    }
}


void list_db(item_t *items, int no_items) // rätt fil??
{
  char *name = items->name;

  for (int i = 0; i < no_items; i++)
  {
    name = items[i].name;
    printf("%d. %s\n", i + 1, name);
  }
}


item_t input_merch(void) // rätt fil??
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


