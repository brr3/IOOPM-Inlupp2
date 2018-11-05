#ifndef ELEM_H
#define ELEM_H
#include "logic.h"

//
// STORAGE OPERATIONS
//

/// @brief Retrieve items data structure
/// @param storage | See comments in ui.h
ioopm_hash_table_t *get_storage_items(storage_t storage);

/// @brief Retrieve locations data structure
/// @param storage | See comments in ui.h
ioopm_hash_table_t *get_storage_locations(storage_t storage);

/// @brief Retrieve the list of all shopping carts
/// @param storage | See comments in ui.h
ioopm_list_t *get_storage_carts(storage_t storage);

/// @brief Retrieve the global shopping cart identifier
/// @param storage | See comments in ui.h
int get_storage_cart_counter(storage_t storage);

/// @brief Retrieve the total amount of items in store
/// @param storage | See comments in ui.h
int get_storage_items_amount(storage_t storage);

/// @brief Retrieve the total amount of shopping carts in the system
/// @param storage | See comments in ui.h
int get_storage_carts_amount(storage_t storage);

/// @brief Overwrite the items data structure
/// @param storage | See comments in ui.h
/// @param items   | The new items data structure to use
void set_storage_items(storage_t *storage, ioopm_hash_table_t *items);

/// @brief Overwrite the locations data structure
/// @param storage   | See comments in ui.h
/// @param locations | The new locations data structure to use
void set_storage_locations(storage_t *storage, ioopm_hash_table_t *locations);

/// @brief Overwrite the shopping carts data structure
/// @param storage | See comments in ui.h
/// @param carts   | The new shopping carts data structure to use
void set_storage_carts(storage_t *storage, ioopm_list_t *carts);

/// @brief Update the global shopping cart identifier
/// @param storage      | See comments in ui.h
/// @param cart_counter | The new identifier to use
void set_storage_cart_counter(storage_t *storage, int cart_counter);

/// @brief Increase the global shopping cart identifier by 1
/// @param storage | See comments in ui.h
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
