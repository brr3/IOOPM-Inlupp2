#ifndef LOGIC_H
#define LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include "../data_structures/linked_list.h"
#include "../data_structures/hash_table.h"
#include "../utils/utils.h"


typedef struct item item_t;
typedef struct shelf shelf_t;

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


void free_hash_table_keys_values(elem_t key_ignored, elem_t value, void *x_ignored);

int hash_string(elem_t key);

bool cmp_string(elem_t a, elem_t b);

bool is_shelf(char *shelf);

bool is_menu_key(char *key);

bool is_yn_key(char *key);

bool no_merch(int merch_count);

bool merch_exists(ioopm_hash_table_t *items, char *name);

item_t *make_merch(char *name, char *desc, int price);

void remake_merch(ioopm_hash_table_t *items, item_t *item);

shelf_t *make_shelf(char *shelf_name, int amount);

shelf_t *find_shelf_in_list(ioopm_list_t *item_locations, char *shelf_name, int *index);

char *to_upper(char *str); // NOT WORKING

void add_item_to_storage(ioopm_hash_table_t *items, item_t *item);

void remove_item_from_storage(ioopm_hash_table_t *items, item_t *item);

void storage_names_to_sorted_array(ioopm_hash_table_t *items, char *arr_names[]);

item_t *extract_item_from_storage(ioopm_hash_table_t *items, int id, char *arr_names[], elem_t *found_element);

void add_shelf_to_list(ioopm_list_t *item_locations, shelf_t *shelf);

void add_shelf_to_ht(ioopm_hash_table_t *locations, shelf_t *shelf);

void print_item(item_t item, int id, bool print_stock);

#endif
