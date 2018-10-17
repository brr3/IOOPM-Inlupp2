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

char *get_shelf_name(shelf_t shelf)
{
  return shelf.shelf_name;
}

int get_shelf_amount(shelf_t shelf)
{
  return shelf.amount;
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

void set_item_shelves(item_t *item, ioopm_list_t *shelves)
{
  item->shelves = shelves;
}

void set_shelf_name(shelf_t *shelf, char *shelf_name)
{
  shelf->shelf_name = shelf_name;
}

void increase_shelf_amount(shelf_t *shelf, int amount)
{
  shelf->amount += amount;
}

