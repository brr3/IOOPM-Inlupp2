#include <stdio.h>
#include "../logic/logic.h"

/// @brief Start the program
void event_loop();

/// @brief Let the user enter new information for a new merchandise to be added to storage
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void add_merch(storage_t *storage);

/// @brief List the full information of each merchandise found in items, depending on print_stock parameter
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
/// @param print_stock | True: print the stock AND basic information of every merchandise
///                    / False: print only the basic information of every merchandise
void list_merch(storage_t *storage, bool print_stock);

/// @brief Let the user choose a merchandise to remove and then remove it permanently from items
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void remove_merch(storage_t *storage);

/// @brief Let the user choose a merchandise to edit, then ask for new information and update items
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void edit_merch(storage_t *storage);

/// @brief Let the user choose a merchandise to show the stock from 
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void show_stock(storage_t *storage);

/// @brief Let the user choose a merchandise to replenish the stock of, then ask for a storage location and amount to increase stock by, and update items
/// @param storage | A storage containing two hash tables, one that maps names of merchandise to the address of the full information for that merchandise, and one that maps names of storage locations to the name of the merchandise stored at that location
void replenish_stock(storage_t *storage);
