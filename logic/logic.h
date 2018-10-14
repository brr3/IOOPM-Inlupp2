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

item_t edit_db(item_t *items, int no_items);

void add_merch_to_db(ioopm_hash_table_t *items);

item_t remove_item_from_db(item_t *items, int no_items);

void list_db(item_t *items, int no_items);

#endif
