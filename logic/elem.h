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

/// @brief Initiate the items data structure
/// @param storage | See comments in ui.h
/// @param items   | The new items data structure to use
void set_storage_items(storage_t *storage, ioopm_hash_table_t *items);

/// @brief Initiate the locations data structure
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

/// @brief Retrieve the name of an item
/// @param item | The item
char *get_item_name(item_t item);

/// @brief Retrieve the description of an item
/// @param item | The item
char *get_item_desc(item_t item);

/// @brief Retrieve the price of an item
/// @param item | The item
int get_item_price(item_t item);

/// @brief Retrieve the list of shelves where an item is stored
/// @param item | The item
ioopm_list_t *get_item_shelves(item_t item);

/// @brief Retrieve the amount of locations an item is stored on
/// @param item | The item
int get_item_shelves_count(item_t item);

/// @brief Retrieve the total stock of an item
/// @param item | The item
int get_item_stock(item_t item);

/// @brief Set the name of an item
/// @param item | The item
/// @param name | The new name
void set_item_name(item_t *item, char *name);

/// @brief Set the description of an item
/// @param item | The item
/// @param desc | The new description
void set_item_desc(item_t *item, char *desc);

/// @brief Set the price of an item
/// @param item  | The item
/// @param price | The new price
void set_item_price(item_t *item, int price);

/// @brief Initiate the list of shelves for an item
/// @param item    | The item
/// @param shelves | The list of shelves to assign to the item
void set_item_shelves(item_t *item, ioopm_list_t *shelves);

//
// SHELF OPERATIONS
//

/// @brief Retrieve the name of a shelf
/// @param shelf | The shelf
char *get_shelf_name(shelf_t shelf);

/// @brief Retrieve the stock on a shelf
/// @param shelf | The shelf
int get_shelf_stock(shelf_t shelf);

/// @brief Set the name of a shelf
/// @param shelf      | The shelf
/// @param shelf_name | The new name
void set_shelf_name(shelf_t *shelf, char *shelf_name);

/// @brief Set the stock on a shelf
/// @param shelf  | The shelf
/// @param amount | The new stock
void set_shelf_stock(shelf_t *shelf, int amount);

/// @brief Increase stock on a shelf by amount parameter
/// @param shelf  | The shelf
/// @param amount | Amount to increase stock by
void increase_shelf_stock(shelf_t *shelf, int amount);

//
// CART OPERATIONS
//

/// @brief Retrieve the id of a shopping cart
/// @param cart | The shopping cart
int get_cart_id(cart_t cart);

/// @brief Retrieve the list of items that have been added to a shopping cart
/// @param cart | The shopping cart
ioopm_list_t *get_cart_items(cart_t cart);

/// @brief Retrieve the name of an item in the shopping cart
/// @param cart_item | The shopping cart item
char *get_cart_item_name(cart_item_t cart_item);

/// @brief Retrieve the quantity of an item in the shopping cart
/// @param cart_item | The shopping cart item
int get_cart_item_quantity(cart_item_t cart_item);

/// @brief Retrieve the amount of items that have been added to a shopping cart
/// @param cart | The shopping cart
int get_cart_items_amount(cart_t cart);

/// @brief Retrieve an item from a shopping cart
/// @param cart_item | The shopping cart item
/// @param item_id   | The id of an item to search for and retrieve
cart_item_t *get_cart_item_from_cart(cart_t cart, int item_id);

/// @brief Retrieve the total cost of all items in a shopping cart
/// @param storage   | See comments in ui.h
/// @param cart_item | The shopping cart item
int get_total_cost(storage_t storage, cart_t cart);

/// @brief Set the id number for a cart
/// @param cart | The shopping cart
/// @param id   | A monotonically increasing id number, must be increased before each function call
void set_cart_id(cart_t *cart, int id);

/// @brief Assign a list of items in a shopping cart to a shopping cart
/// @param cart       | The shopping cart
/// @param cart_items | The list of items to assign
void set_cart_items(cart_t *cart, ioopm_list_t *cart_items);

/// @brief Set the name of an item in a shopping cart
/// @param cart_item | The shopping cart item
/// @param name      | The new name
void set_cart_item_name(cart_item_t *cart_item, char *name);

/// @brief Set the quantity of an item in a shopping cart
/// @param cart_item | The shopping cart item
/// @param quantity  | The new quantity
void set_cart_item_quantity(cart_item_t *cart_item, int quantity);

/// @brief Increase the quantity of an item in a shopping cart by quantity parameter
/// @param cart_item | The shopping cart item
/// @param quantity  | The quantity to increment stock with
void increase_cart_item_quantity(cart_item_t *cart_item, int quantity);

#endif
