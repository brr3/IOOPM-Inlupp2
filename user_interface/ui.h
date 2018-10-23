#include <stdio.h>
#include "../logic/logic.h"

/// @brief Start the program
void event_loop();

/// @brief Let the user enter new information for a new merchandise to be added to items
/// @param items | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
void add_merch(ioopm_hash_table_t *items);

/// @brief List the full information of each merchandise found in items, depending on print_stock parameter
/// @param items       | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
/// @param print_stock | True: print the stock AND basic information of every merchandise
///                    / False: print only the basic information of every merchandise
void list_merch(ioopm_hash_table_t *items, bool print_stock);

/// @brief Let the user choose a merchandise to remove and then remove it permanently from items
/// @param items | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
void remove_merch(ioopm_hash_table_t *items);

/// @brief Let the user choose a merchandise to edit, then ask for new information and update items
/// @param items | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
void edit_merch(ioopm_hash_table_t *items);

/// @brief Let the user choose a merchandise to show the stock from 
/// @param items | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
void show_stock(ioopm_hash_table_t *items);

/// @brief Let the user choose a merchandise to replenish the stock of, then ask for a storage location and amount to increase stock by, and update items
/// @param items | A hash table where each key is the name of the merchandise and each value is an address to the full information of that merchandise
void replenish_stock(ioopm_hash_table_t *items);
