#ifndef LOGIC_H
#define LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include "../data_structures/linked_list.h"
#include "../data_structures/hash_table.h"
#include "../utils/utils.h"

typedef struct storage storage_t;
typedef struct item item_t;
typedef struct shelf shelf_t;
typedef struct cart cart_t;
typedef struct cart_item cart_item_t;

/// @brief Main storage containing everything needed for the program to run
struct storage
{
  ioopm_hash_table_t *items;
  ioopm_hash_table_t *locations;
  ioopm_list_t *carts;
  int cart_counter;
};

/// @brief Item stored in storage with a name, description, price and a list of shelves
struct item
{
  char *name;
  char *desc;
  int price;
  ioopm_list_t *shelves;
};

/// @brief Shelf for keeping track of stock, stored in shelves list in an item
struct shelf
{
  char *shelf_name;
  int stock;
};

/// @brief Shopping cart with a monotonically increasing id number and a list of items that are in the shopping cart
struct cart
{
  int id;
  ioopm_list_t *cart_items;
};

/// @brief Shopping cart item stored in a shopping cart with a quantity associated
struct cart_item
{
  char *name;
  int quantity;
};

//
// MEMORY MANAGEMENT
//

/// @brief Free all memory allocated by storage. Must be run before program quits.
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void destroy_storage(storage_t *storage);

//
// MENU LOGIC
//

/// @brief Check if the entered string is a valid shelf
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_shelf(char *shelf);

/// @brief Check if the entered string begins with a valid menu key
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_menu_key(char *key);

/// @brief Check if the entered string begins with y, Y, n or N
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_yn_key(char *key);

//
// STORAGE LOGIC
//

/// @brief Initialise the storage. Must be run before program starts.
/// @return | The initialised storage
storage_t *make_storage();

//
// ITEM LOGIC
//

/// @brief Create a new item with the given parameters
/// @param name  | Name of the item
/// @param desc  | Description of the item
/// @param price | Price of the item
/// @return      | A newly created item
item_t *make_item(char *name, char *desc, int price);

/// @brief Check if an item exists in storage
/// @param storage    | A storage containing two hash tables, one that maps names of items to
///                   | ...the address of the full information for that item, one that maps names
///                   | ...of storage locations to the name of the item stored at that location,
///                   | ...and a linked list of shopping carts
/// @param merch_name | The name of the item to check
/// @return           | True if item exists in storage, else false
bool item_exists(storage_t storage, char *item_name);

/// @brief Transfer details of an existing item to a newly allocated item and add it
///                | to storage. Must be run after removing an existing item from storage in
///                | conjunction with changing the name of that item
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param item    | The item to transfer details from 
void remake_item(storage_t *storage, item_t *item);

/// @brief Add item to storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param item    | The item to add
void add_item_to_storage(storage_t *storage, item_t *item);

/// @brief Remove item from storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param item    | The item to remove
void remove_item_from_storage(storage_t *storage, item_t *item);

/// @brief Sort all item names into an array
/// @param storage   | A storage containing two hash tables, one that maps names of items to
///                  | ...the address of the full information for that item, one that maps names
///                  | ...of storage locations to the name of the item stored at that location,
///                  | ...and a linked list of shopping carts
/// @param arr_names | The array to add all merchandise names to, should fit the total amount of items
///                  | ...in storage
void item_names_to_sorted_array(storage_t storage, char *arr_names[]);

/// @brief Extract the address to the full information of the found item from items
/// @param storage     | A storage containing two hash tables, one that maps names of items to
///                    | ...the address of the full information for that item, one that maps names
///                    | ...of storage locations to the name of the item stored at that location,
///                    | ...and a linked list of shopping carts
/// @param item_name   | Name of the merchandise to retrieve
/// @param found_value | Value to be overwritten by the address of the full information for the merchandise,
///                    | ...can be NULL but then you MUST dereference the return value before use
/// @return            | The address to the found merchandise
item_t *extract_item_from_storage(storage_t storage, char *item_name, elem_t *found_value);

//
// SHELF LOGIC
//

/// @brief Check if a storage location exists
/// @param storage     | A storage containing two hash tables, one that maps names of items to
///                    | ...the address of the full information for that item, one that maps names
///                    | ...of storage locations to the name of the item stored at that location,
///                    | ...and a linked list of shopping carts
/// @param shelf_name  | The name of the shelf
/// @param found_value | Value to be overwritten by the address of the full information for the shelf
///                    | ...can be NULL
/// @return            | True if storage location is found, else false
bool location_exists(storage_t storage, char *shelf_name, elem_t *found_value);

/// @brief Create a shelf and add it to storage
/// @param item       | Item to update with shelf location
/// @param shelf_name | Name of the shelf to create
/// @param stock      | Stock of the shelf to create
void add_shelf_to_storage(storage_t *storage, item_t *item, char *shelf_name, int stock);

/// @brief Find the specified shelf in item
/// @param item               | Item to extract item shelves from
/// @param shelf_name_to_find | Name of the shelf sought
/// @return                   | The found shelf
shelf_t *find_shelf_in_item(item_t *item, char *shelf_name_to_find);

/// @brief Checkout all items in a cart
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param cart    | Shopping cart to checkout
void checkout_cart_items(storage_t *storage, cart_t *cart);

//
// CART LOGIC
//

/// @brief Check if a shopping cart exists
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param cart_id | The identification number of the shopping cart to check for
/// @return        | True if a cart is found, else false
bool cart_exists(storage_t storage, int cart_id);

/// @brief Add a new shopping cart to storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void add_cart_to_storage(storage_t *storage);

/// @brief Remove a shopping cart from storage         
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param cart_id | The identification number of the shopping cart to remove
void remove_cart_from_storage(storage_t *storage, int cart_id);

/// @brief Retrieve a shopping cart from storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param cart_id | The identification number of the shopping cart to retrieve 
cart_t *extract_cart_from_storage(storage_t storage, int cart_id);

/// @brief Add an item to a shopping cart
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
/// @param item    | The item to add
/// @param amount  | Amount of item to add
/// @param cart_id | The identification number of the shopping cart to add item to
void add_item_to_cart(storage_t storage, item_t item, int amount, int cart_id);

/// @brief Remove an item from a shopping cart
/// @param cart    | The shopping cart
/// @param item_id | The identification number of the item to remove
void remove_item_from_cart(cart_t *cart, int item_id);

//
// PRINT FUNCTIONS
//

/// @brief Retrieve all information from an item and print it to screen
/// @param item        | The item to retrieve information from
/// @param id          | A unique identification number to print together with the item
/// @param print_stock | True: print the stock >>and<< basic information of the item
///                    | False: print only the basic information of the item
void print_item(item_t item, int id, bool print_stock);

/// @brief Retrieve all information from a shopping cart and print it to screen
/// @param cart | The shopping cart to print
void print_cart(cart_t cart);

#endif
