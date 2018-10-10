#include <stdio.h>
#include <stdbool.h>
#include "../data_structures/linked_list.h"
#include "../data_structures/hash_table.h"

typedef struct item item_t;

struct item
{
  char *name;
  char *desc;
  int price;
  ioopm_hash_table_t *shelves;
  int amount;
};


char *get_item_name(item_t item);

char *get_item_desc(item_t item);

int get_item_price(item_t item);

ioopm_list_t *get_item_shelves(item_t item);

int get_item_amount(item_t item);


item_t make_merch(char *name, char *desc, int price, char *shelf);

bool is_shelf(char *shelf);

bool is_menu_key(char *key);


item_t edit_db(item_t *items, int no_items);

void add_merch_to_db(ioopm_hash_table_t *items);

item_t remove_item_from_db(item_t *items, int no_items);


void list_db(item_t *items, int no_items);

item_t input_merch(void);

void print_item(item_t i);
