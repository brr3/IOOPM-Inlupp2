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

struct storage
{
  ioopm_hash_table_t *items;
  ioopm_hash_table_t *locations;
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

/// @brief Free all data stored in memory by merchandise
/// @param items     | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
/// @param locations | A hash table where each key is the name of a storage location and each value is the name of the merchandise stored at that location 
void destroy_storage(storage_t *storage);

int hash_string(elem_t key);

bool cmp_string(elem_t a, elem_t b);

bool is_shelf(char *shelf);

bool is_menu_key(char *key);

bool is_yn_key(char *key);

storage_t *make_storage();

bool merch_exists(ioopm_hash_table_t *items, char *name);

item_t *make_merch(char *name, char *desc, int price);

void remake_merch(ioopm_hash_table_t *items, item_t *item);

shelf_t *make_shelf(char *shelf_name, int amount);

shelf_t *find_shelf_in_list(ioopm_list_t *item_locations, char *shelf_name, int *index);

char *to_upper(char *str);

void add_item_to_storage(ioopm_hash_table_t *items, item_t *item);

void remove_item_from_storage(storage_t *storage, item_t *item);

void storage_names_to_sorted_array(ioopm_hash_table_t *items, char *arr_names[]);

/// @brief Extract the address to the full information of the found merchandise from items
/// @param items       | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
/// @param item_name   | Name of the merchandise
/// @param found_value | Overwritten by the address to the full information of the merchandise, can be NULL but then you MUST dereference the return value before use
/// @returns The address to the found merchandise
item_t *extract_item_from_storage(ioopm_hash_table_t *items, char *item_name, elem_t *found_value);

void add_shelf_to_item_shelves(ioopm_list_t *item_shelves, shelf_t *shelf);

void add_shelf_to_locations(ioopm_hash_table_t *locations, shelf_t *shelf);

/// @brief Extract all information from a merchandise and print it to screen
/// @param item        | The merchandise to extract information from
/// @param id          | A unique identification number to print together with the merchandise
/// @param print_stock | True: print the stock AND basic information of the merchandise
///                    / False: print only the basic information of  merchandise
void print_item(item_t item, int id, bool print_stock);

#endif
