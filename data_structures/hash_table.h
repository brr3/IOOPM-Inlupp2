#pragma once

#include "linked_list.h"
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct hash_table ioopm_hash_table_t;

/// @brief: create a new hash table with a specific size and load factor
/// @param: hash_fun - function to use to get hashed key of input data, assumes keys are ints if NULL
/// @param: eq_fun - function to use to compare keys, assumes values are strings if NULL
/// @param: no_buckets - size of hash table
/// @param: load_factor - threshold of hash table loading before increasing size (e.g. 0.5)
/// @return: a new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create_custom(ioopm_hash_function hash_fun,
                                                        ioopm_eq_function eq_fun,
                                                        size_t no_buckets,
                                                        float load_factor);

/// @brief: create a new hash table with a size 17 and load factor 0.75
/// @param: hash_fun - function to use to get hashed key of input data, assumes keys are ints if NULL
/// @param: eq_fun - function to use to compare keys, assumes values are strings if NULL
/// @return: a new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun);

/// @brief: add key => value entry in hash table ht
/// @param: ht - hash table operated upon
/// @param: key - key to insert
/// @param: value - value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief: lookup value for key in hash table ht
/// @param: ht - hash table operated upon
/// @param: key - key to lookup
/// @param: lookup - pointer to element, will change to value of element if found, else unchanged
/// @return: true if entry exists, else false
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief: remove any mapping from key to a value
/// @param: ht - hash table operated upon
/// @param: key - key to remove
/// @return: value mapped to by key, NULL if not found
elem_t ioopm_hash_table_remove_entry(ioopm_hash_table_t *ht, elem_t key);

/// @brief: deallocates the whole hash table and all its entries
/// @param: ht - hash table operated upon
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief: returns the number of key => value entries in the hash table
/// @param: h - hash table operated upon
/// @return: the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *h);

/// @brief: checks if the hash table is empty
/// @param: h -  hash table operated upon
/// @return: true if size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *h);

/// @brief clear all the entries in a hash table
/// @param h - hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *h);

/// @brief: return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param: h - hash table operated upon
/// @return: a list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *h);

/// @brief: return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param: h - hash table operated upon
/// @return: a list of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *h); 

/// @brief: check if a hash table has an entry with a given key
/// @param: h - hash table operated upon
/// @param: key - the key sought 
bool ioopm_hash_table_has_key(ioopm_hash_table_t *h, elem_t key);

/// @brief: check if a hash table has an entry with a given value
/// @param: h - hash table operated upon
/// @param: value - the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *h, ioopm_eq_function value_cmp, elem_t value);

/// @brief: check if a predicate is satisfied by all entries in a hash table
/// @param: h - hash table operated upon
/// @param: pred - the predicate 
/// @param: arg - extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *h, ioopm_predicate pred, void *arg);

/// @brief: check if a predicate is satisfied by any entry in a hash table
/// @param: h - hash table operated upon
/// @param: pred - the predicate 
/// @param: arg - extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *h, ioopm_predicate pred, void *arg);

/// @brief: apply a function to all entries in a hash table
/// @param: h - hash table operated upon
/// @param: apply_fun - the function to be applied to all elements
/// @param: arg - extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *h, ioopm_apply_function apply_fun, void *arg);
