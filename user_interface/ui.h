#include <stdio.h>
#include "../logic/logic.h"

void event_loop();

void add_merch(ioopm_hash_table_t *items);

void list_merch(ioopm_hash_table_t *item, bool print_stock);

void remove_merch(ioopm_hash_table_t *items);

void edit_merch(ioopm_hash_table_t *items);

void show_stock(ioopm_hash_table_t *items);

void replenish_stock(ioopm_hash_table_t *items);
