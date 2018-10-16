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

int hash_string(elem_t key);

bool cmp_string(elem_t a, elem_t b);

bool is_shelf(char *shelf);

bool is_menu_key(char *key);

bool merch_exists(ioopm_hash_table_t *items, char *name);

item_t *make_merch(char *name, char *desc, int price);

item_t *remake_merch(item_t item, char *new_name);

char *to_upper(char *str);

void sort_keys(char *keys[], size_t no_keys);

void ht_names_to_sorted_array(ioopm_hash_table_t *items, char *arr[]);

void print_item(item_t item, int id, bool print_stock);

#endif
