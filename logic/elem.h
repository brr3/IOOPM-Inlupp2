#ifndef ELEM_H
#define ELEM_H
#include "logic.h"

char *get_item_name(item_t item);

char *get_item_desc(item_t item);

int get_item_price(item_t item);

ioopm_list_t *get_item_shelves(item_t item);

int get_item_shelves_count(item_t item);

int get_item_total_amount(item_t item);

void set_item_name(item_t *item, char *name);

void set_item_desc(item_t *item, char *desc);

void set_item_price(item_t *item, int price);

#endif
