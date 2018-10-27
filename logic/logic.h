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

struct storage
{
  ioopm_hash_table_t *items;
  ioopm_hash_table_t *locations;
  ioopm_list_t *carts;
  int cart_id;
};

struct item
{
  char *name;
  char *desc;
  int price;
  ioopm_list_t *shelves;
};

struct shelf
{
  char *shelf_name;
  int amount;
};

struct cart
{
  int id;
  ioopm_list_t *cart_items;
};

struct cart_item
{
  char *name;
  int amount;
};

//
// MEMORY MANAGEMENT
//

/// @brief         | Free all memory allocated by storage. Must be run before program quits.
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void destroy_storage(storage_t *storage);

//
// MENU LOGIC
//

/// @brief     | Check if the entered string is a valid shelf
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_shelf(char *shelf);

/// @brief     | Check if the entered string begins with a valid menu key
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_menu_key(char *key);

/// @brief     | Check if the entered string begins with y, Y, n or N
/// @param key | Key to check
/// @return    | True if key is valid, else false
bool is_yn_key(char *key);

//
// STORAGE LOGIC
//

/// @brief  | Initialise the storage. Must be run before program starts.
/// @return | The initialised storage
storage_t *make_storage();

//
// ITEM LOGIC
//

/// @brief       | Create a merchandise with the given parameters
/// @param name  | Name of the merchandise
/// @param desc  | Description of the merchandise
/// @param price | Price of the merchandise
/// @return      | A newly created merchandise
item_t *make_item(char *name, char *desc, int price);

/// @brief            | Check if a merchandise exists in storage
/// @param storage    | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param merch_name | The name of the merchandise to check
/// @return           | True if merchandise exists in storage, else false
bool item_exists(storage_t *storage, char *item_name);

/// @brief         | Transfer details of an existing merchandise to a newly allocated merchandise and add it to storage. Must be run after removing an existing merchandise from storage in conjunction with changing the name of that merchandise.
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param item    | The merchandise to transfer details from 
void remake_item(storage_t *storage, item_t *item);

/// @brief         | Add item to storage
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param item    | The item to add
void add_item_to_storage(storage_t *storage, item_t *item);

/// @brief         | Remove item from storage
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param item    | The item to remove
void remove_item_from_storage(storage_t *storage, item_t *item);

/// @brief           | Sort all merchandise names into an array
/// @param storage   | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param arr_names | The array to add all merchandise names to, should fit the total amount of merchandise in storage
void storage_names_to_sorted_array(storage_t *storage, char *arr_names[]);

/// @brief Extract the address to the full information of the found merchandise from items
/// @param storage     | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param item_name   | Name of the merchandise to retrieve
/// @param found_value | Value overwritten by the address of the full information for the merchandise, can be NULL but then you must >>dereference the return value before use<<
/// @returns           | The address to the found merchandise
item_t *extract_item_from_storage(storage_t *storage, char *item_name, elem_t *found_value);

//
// SHELF LOGIC
//

/// @brief        | Create a shelf with the given parameters
/// @param name   | Name of the shelf
/// @param amount | Amount of merchandise to be stored on the shelf
/// @return       | A newly created shelf
shelf_t *make_shelf(char *shelf_name, int amount);

/// @brief              | Add shelf to the list of shelves for a particular merchandise
/// @param item_shelves | A linked list where each element is a pointer to a shelf
/// @param shelf        | Shelf to add
void add_shelf_to_item_shelves(ioopm_list_t *item_shelves, shelf_t *shelf);

/// @brief              | Insert the name of a merchandise into the second hash table mentioned below
/// @param storage      | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param shelf_name   | Name of shelf to use as key
/// @param item_name    | Name of the merchandise to be inserted into the hash table 
void add_shelf_to_locations(storage_t *storage, char *shelf_name, char *item_name);

/// @brief              | Find the specified shelf in a list of shelves
/// @param item_shelves | List operated upon
/// @param shelf_name   | Name of the shelf sought
/// @param index        | The index of the found shelf in the list, visible outside the function
/// @return             | The found shelf
shelf_t *find_shelf_in_item_shelves(ioopm_list_t *item_shelves, char *shelf_name, int *index);

//
// CART LOGIC
//

bool cart_exists(storage_t *storage, int id);

void add_cart_to_storage(storage_t *storage);

void remove_cart_from_storage(storage_t *storage, int id);

cart_t *extract_cart_from_storage(storage_t *storage, int cart_id);

void add_cart_item_to_cart(storage_t *storage, item_t item, int amount, int cart_id);

//
// PRINT FUNCTIONS
//

/// @brief Extract all information from a merchandise and print it to screen
/// @param item        | The merchandise to extract information from
/// @param id          | A unique identification number to print together with the merchandise
/// @param print_stock | True: print the stock >>and<< basic information of the merchandise
///                    | False: print only the basic information of the merchandise
void print_item(item_t item, int id, bool print_stock);

void print_cart(cart_t cart);

#endif
