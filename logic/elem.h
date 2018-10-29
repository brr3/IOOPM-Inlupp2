#ifndef ELEM_H
#define ELEM_H
#include "logic.h"

//
// STORAGE OPERATIONS
//

ioopm_hash_table_t *get_storage_items(storage_t storage);

ioopm_hash_table_t *get_storage_locations(storage_t storage);

ioopm_list_t *get_storage_carts(storage_t storage);

int get_storage_cart_counter(storage_t storage);

int get_storage_items_amount(storage_t storage);

int get_storage_carts_amount(storage_t storage);

void set_storage_items(storage_t *storage, ioopm_hash_table_t *items);

void set_storage_locations(storage_t *storage, ioopm_hash_table_t *locations);

void set_storage_carts(storage_t *storage, ioopm_list_t *carts);

void set_storage_cart_counter(storage_t *storage, int cart_counter);

int increase_cart_counter(storage_t *storage);

//
// ITEM OPERATIONS
//

char *get_item_name(item_t item);

char *get_item_desc(item_t item);

int get_item_price(item_t item);

ioopm_list_t *get_item_shelves(item_t item);

int get_item_shelves_count(item_t item);

int get_item_stock(item_t item);

void set_item_name(item_t *item, char *name);

void set_item_desc(item_t *item, char *desc);

void set_item_price(item_t *item, int price);

void set_item_shelves(item_t *item, ioopm_list_t *shelves);

//
// SHELF OPERATIONS
//

char *get_shelf_name(shelf_t shelf);

int get_shelf_stock(shelf_t shelf);

void set_shelf_name(shelf_t *shelf, char *shelf_name);

void set_shelf_stock(shelf_t *shelf, int amount);

void increase_shelf_stock(shelf_t *shelf, int amount);

//
// CART OPERATIONS
//

int get_cart_id(cart_t cart);

ioopm_list_t *get_cart_items(cart_t cart);

char *get_cart_item_name(cart_item_t cart_item);

int get_cart_item_quantity(cart_item_t cart_item);

int get_cart_items_amount(cart_t cart);

cart_item_t *get_cart_item_from_cart(cart_t cart, int item_id);

int get_total_cost(storage_t storage, cart_t cart);

void set_cart_id(cart_t *cart, int id);

void set_cart_items(cart_t *cart, ioopm_list_t *cart_items);

void set_cart_item_name(cart_item_t *cart_item, char *name);

void set_cart_item_quantity(cart_item_t *cart_item, int quantity);

void increase_cart_item_quantity(cart_item_t *cart_item, int quantity);

#endif
