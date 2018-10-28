#include "elem.h"
#include "logic.h"

//
// STORAGE OPERATIONS
//

ioopm_hash_table_t *get_storage_items(storage_t storage)
{
  return storage.items;
}

ioopm_hash_table_t *get_storage_locations(storage_t storage)
{
  return storage.locations;
}

ioopm_list_t *get_storage_carts(storage_t storage)
{
  return storage.carts;
}

int get_storage_cart_counter(storage_t storage)
{
  return storage.cart_counter;
}

int get_storage_items_amount(storage_t storage)
{
  return ioopm_hash_table_size(get_storage_items(storage));
}

int get_storage_carts_amount(storage_t storage)
{
  return ioopm_linked_list_size(get_storage_carts(storage)); 
}

int increase_cart_counter(storage_t *storage)
{
  return storage->cart_counter += 1;
}

//
// ITEM OPERATORS
//

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

int get_item_stock(item_t item)
{
  ioopm_list_t *shelves = get_item_shelves(item);
  int stock = 0;
  for (int i = 0; i < get_item_shelves_count(item); i++)
    {
      shelf_t *shelf = ioopm_linked_list_get(shelves, i).v;
      stock += shelf->stock;
    }
  return stock;
}

void set_item_name(item_t *item, char *name)
{
  free(item->name);
  item->name = name;
}

void set_item_desc(item_t *item, char *desc)
{
  free(item->desc);
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

//
// SHELF OPERATORS
//

char *get_shelf_name(shelf_t shelf)
{
  return shelf.shelf_name;
}

int get_shelf_stock(shelf_t shelf)
{
  return shelf.stock;
}

void set_shelf_name(shelf_t *shelf, char *shelf_name)
{
  shelf->shelf_name = shelf_name;
}

void set_shelf_stock(shelf_t *shelf, int amount)
{
  shelf->stock = amount;
}

void increase_shelf_stock(shelf_t *shelf, int amount)
{
  shelf->stock += amount;
}

//
// CART OPERATIONS
//

int get_cart_id(cart_t cart)
{
  return cart.id;
}

ioopm_list_t *get_cart_items(cart_t cart)
{
  return cart.cart_items;
}

char *get_cart_item_name(cart_item_t cart_item)
{
  return cart_item.name;
}

int get_cart_item_quantity(cart_item_t cart_item)
{
  return cart_item.quantity;
}

int get_cart_items_amount(cart_t cart)
{
  ioopm_list_t *cart_items = get_cart_items(cart);
  return ioopm_linked_list_size(cart_items);
}

cart_item_t *get_cart_item_from_cart(cart_t cart, int item_id)
{
  ioopm_list_t *cart_items = get_cart_items(cart);
  return ioopm_linked_list_get(cart_items, item_id - 1).v;
}

void set_cart_id(cart_t *cart, int id)
{
  cart->id = id;
}

void set_cart_items(cart_t *cart, ioopm_list_t *cart_items)
{
  cart->cart_items = cart_items;
}

void set_cart_item_name(cart_item_t *cart_item, char *name)
{
  cart_item->name = name;
}

void set_cart_item_quantity(cart_item_t *cart_item, int quantity)
{
  cart_item->quantity = quantity;
}

void increase_cart_item_quantity(cart_item_t *cart_item, int quantity)
{
  cart_item->quantity += quantity;
}



