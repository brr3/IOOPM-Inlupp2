#include <stdio.h>
#include "../logic/logic.h"

/// @brief Start the program
void event_loop();

/// @brief Let the user enter new information for a new merchandise to be added to storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void add_merch(storage_t *storage);

/// @brief List the full information of each merchandise found in storage
/// @param storage     | A storage containing two hash tables, one that maps names of items to
///                    | ...the address of the full information for that item, one that maps names
///                    | ...of storage locations to the name of the item stored at that location,
///                    | ...and a linked list of shopping carts
/// @param print_stock | True: print the stock AND basic information of every merchandise
///                    / False: print only the basic information of every merchandise
void list_merch(storage_t *storage, bool print_stock);

/// @brief Let the user choose a merchandise to remove and then remove it permanently from storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void remove_merch(storage_t *storage);

/// @brief Let the user choose a merchandise to edit, then ask for new information and update storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void edit_merch(storage_t *storage);

/// @brief Let the user choose a merchandise and show the stock of that merchandise
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void show_stock(storage_t *storage);

/// @brief Let the user choose a merchandise to replenish the stock of, then ask for a storage
/// ...location and the amount to increase stock by, before updating storage
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void replenish_stock(storage_t *storage);

/// @brief Create a shopping cart
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void create_cart(storage_t *storage);

/// @brief Let the user choose a shopping cart to remove from the system, then remove it
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void remove_cart(storage_t *storage);

/// @brief Let the user choose an item from storage, ask for a quantity, let the user choose a
/// ...shopping cart, then add selected item and quantity to the selected shopping cart
/// @param storage | A storage containing two hash tables, one that maps names of items to
///                | ...the address of the full information for that item, one that maps names
///                | ...of storage locations to the name of the item stored at that location,
///                | ...and a linked list of shopping carts
void add_to_cart(storage_t *storage);
