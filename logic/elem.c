#include "elem.h"


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
  return item.shelves;
}

int get_item_shelves_count(item_t item)
{
  return ioopm_linked_list_size(item.shelves);
}

int get_item_total_amount(item_t item)
{
  ioopm_list_t *shelves = get_item_shelves(item);
  int amount = 0;
  for (int i = 0; i < get_item_shelves_count(item); i++)
    {
      shelf_t shelf = *((shelf_t*) ioopm_linked_list_get(shelves, i).v);
      amount += shelf.amount;
    }
  return amount;
}


void set_item_name(item_t *item, char *name)
{
  item->name = name;
}

void set_item_desc(item_t *item, char *desc)
{
  item->desc = desc;
}

void set_item_price(item_t *item, int price)
{
  item->price = price;
}

 // FUNKAR EJ 
void initiate_item_shelves(item_t *item, char *shelf_name)
{
  ioopm_list_t *list = ioopm_linked_list_create(); 
  shelf_t shelf = {.shelf_name = shelf_name, .amount = 0};
  elem_t element = {.v = &shelf};
  ioopm_linked_list_prepend(list, element);
  item->shelves = list;
} 

