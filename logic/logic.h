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

bool is_shelf(char *shelf);

bool is_menu_key(char *key);

bool merch_exists(ioopm_hash_table_t *items, char *name);

item_t *make_merch(char *name, char *desc, int price);

void print_item(item_t item);

#endif
