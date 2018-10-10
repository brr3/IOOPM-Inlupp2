#include "hash_table.h"
#include "common.h"
#include "../linked_list/linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include <assert.h>
#include <string.h>

typedef struct entry entry_t;

entry_t **aggregate_entries(ioopm_hash_table_t *ht);

struct entry
{
  elem_t key;
  elem_t value;
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets;
  ioopm_hash_function hash_fun;
  ioopm_eq_function eq_fun;
  size_t no_buckets;
  float load_factor;
  size_t size;
};

/// @brief: default hash function for int keys
int extract_int_hash_key(elem_t key)
{
  return key.i;
}

/// @brief: default compare function for ints
bool int_equiv(elem_t a, elem_t b)
{
  return a.i == b.i;
}

ioopm_hash_table_t *ioopm_hash_table_create_custom(ioopm_hash_function hash_fun,
                                                   ioopm_eq_function eq_fun,
                                                   size_t no_buckets,
                                                   float load_factor)
{ 
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  
  if (hash_fun == NULL) {
    ht->hash_fun = extract_int_hash_key;
  } else {
    ht->hash_fun = hash_fun; 
  }
  
  if (eq_fun == NULL) {
    ht->eq_fun = int_equiv;
  } else {
    ht->eq_fun = eq_fun;
  }

  ht->no_buckets = no_buckets;
  ht->load_factor = load_factor;
  ht->buckets = calloc(ht->no_buckets, sizeof(entry_t));
  ht->size = 0;
   
  return ht;
}
 
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create_custom(NULL, NULL, 17, 0.75);
  
  return ht;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;
  
  return new_entry;		
}

static entry_t *find_previous_entry_for_key(ioopm_eq_function eq_fun, entry_t *dummy_entry, elem_t key)
{
  entry_t *previous_entry = dummy_entry;

  for (entry_t *cursor = previous_entry->next; cursor; previous_entry = cursor, cursor = cursor->next) {
    if (eq_fun(cursor->key, key)) return previous_entry;
  }

  return previous_entry;
}

//
// HASH TABLE GROWING 
//

static bool load_factor_exceeded(ioopm_hash_table_t *ht)
{
  float cur_load = (float)ioopm_hash_table_size(ht) / ht->no_buckets;
  return (cur_load > ht->load_factor);
}

static size_t calc_new_size(size_t cur_no_buckets)
{
  size_t no_primes = 11;
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

  size_t i = 0;
  for (; cur_no_buckets >= primes[i] && i < no_primes; ++i);
  
  size_t new_no_buckets = primes[i];
  
  return new_no_buckets;
}

static void rehash_hash_table(ioopm_hash_table_t *ht, ioopm_list_t *all_keys, ioopm_list_t *all_values)
{
  for (size_t i = 0; i < ioopm_linked_list_size(all_keys); ++i) {
    ioopm_hash_table_insert(ht, ioopm_linked_list_get(all_keys, i), ioopm_linked_list_get(all_values, i));
  }
}

static void hash_table_grow(ioopm_hash_table_t *ht)
{
  ioopm_list_t *all_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *all_values = ioopm_hash_table_values(ht);
  size_t new_no_buckets = calc_new_size(ht->no_buckets);  

  ioopm_hash_table_clear(ht);
  entry_t *to_free = ht->buckets;
  free(to_free);
  
  ht->no_buckets = new_no_buckets;
  // allocating whole new list of buckets instead of using realloc (calloc seems safer)
  // potential for optimizing speed
  entry_t *new_buckets = calloc(ht->no_buckets, sizeof(entry_t));
  ht->buckets = new_buckets;
  
  rehash_hash_table(ht, all_keys, all_values);
  ioopm_linked_list_destroy(all_keys);
  ioopm_linked_list_destroy(all_values);
}

//

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  if (load_factor_exceeded(ht)) {
    hash_table_grow(ht);
  }
  
  int int_key = abs(ht->hash_fun(key));
  int bucket = int_key % (ht->no_buckets);

  entry_t *prev = find_previous_entry_for_key(ht->eq_fun, &ht->buckets[bucket], key);
  entry_t *cursor = prev->next;

  /// check if the next entry should be updated or not
  if (cursor != NULL && ht->eq_fun(cursor->key, key)) {
    cursor->value = value;
  } else {
    prev->next = entry_create(key, value, cursor);
    ht->size += 1;
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  int int_key = abs(ht->hash_fun(key));

  entry_t *prev = find_previous_entry_for_key(ht->eq_fun, &ht->buckets[int_key % (ht->no_buckets)], key);
  entry_t *cursor = prev->next;;
  
  if (cursor && ht->eq_fun(cursor->key, key)) {
    *result = cursor->value;
    return true;
  } else {
    return false;
  }
}

static void entry_destroy(entry_t **entry)
{
  free(*entry);
  *entry = NULL;
}

elem_t ioopm_hash_table_remove_entry(ioopm_hash_table_t *ht, elem_t key)
{
  int int_key = abs(ht->hash_fun(key));
  int bucket = int_key % (ht->no_buckets);
  
  entry_t *prev = find_previous_entry_for_key(ht->eq_fun, &ht->buckets[bucket], key);
  entry_t *cursor = prev->next;

  if (cursor == NULL) {
    void *ptr = NULL;
    return (elem_t)ptr;
  }
  
  elem_t value = cursor->value;
  prev->next = cursor->next;
  
  entry_destroy(&cursor);
  ht->size -= 1;
  
  return value;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  if (ioopm_hash_table_is_empty(ht)) {
    return;
  }
  
  for (size_t i = 0; i < (ht->no_buckets); ++i) { 
    entry_t *prev = &ht->buckets[i];
    entry_t *cursor = prev->next;

    if (cursor != NULL) {
      prev->next = NULL;
    }
    
    while (cursor) {
      prev = cursor;
      cursor = cursor->next;
      entry_destroy(&prev);
    }
  }

  ht->size = 0;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  free(ht->buckets);
  free(ht);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ht->size == 0;
}

/// @brief: aggregate all entries in ht into a null-terminated list of pointers to entries
/// @param: ht - hash table operated upon
/// @return: null-terminated list of pointers to all entries in ht
entry_t **aggregate_entries(ioopm_hash_table_t *ht)
{
  size_t ht_size = ioopm_hash_table_size(ht);
  entry_t **all_entries = calloc(1, sizeof(char*) * (ht_size + 1));
 
  int array_index = 0;
  for (size_t i = 0; i < (ht->no_buckets); ++i) {

    entry_t *dummy_entry = &ht->buckets[i];
    
    if (dummy_entry->next == NULL) {
      continue;
    }
    
    entry_t *current_entry = dummy_entry->next;
    entry_t *next_entry = current_entry->next;
    
    while (next_entry) {
      all_entries[array_index] = current_entry;
      current_entry = next_entry;
      next_entry = next_entry->next;
      ++array_index;
    }

    all_entries[array_index] = current_entry;
    ++array_index;
  }

  all_entries[array_index] = NULL;
  
  return(all_entries);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  entry_t **all_entries = aggregate_entries(ht);
  ioopm_list_t *all_keys_linked_list = ioopm_linked_list_create(ht->eq_fun);
  
  for (int i = 0; all_entries[i] != NULL; ++i ) {
    ioopm_linked_list_append(all_keys_linked_list, all_entries[i]->key);
  }
  
  free(all_entries);
  return all_keys_linked_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  entry_t **all_entries = aggregate_entries(ht);
  ioopm_list_t *all_values_linked_list = ioopm_linked_list_create(ht->eq_fun);

  for (int i = 0; all_entries[i] != NULL; ++i) {
    ioopm_linked_list_append(all_values_linked_list, all_entries[i]->value);
  }

  free(all_entries);
  return(all_values_linked_list);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  size_t ht_size = ioopm_hash_table_size(ht);
  ioopm_list_t *all_keys = ioopm_hash_table_keys(ht);
  
  bool result = false;
  for (size_t i = 0; i < ht_size && !result; ++i) {
    result = ht->eq_fun(ioopm_linked_list_get(all_keys, i), key);
  }
  
  ioopm_linked_list_destroy(all_keys);
  return result;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, ioopm_eq_function value_cmp, elem_t value)
{  
  size_t ht_size = ioopm_hash_table_size(ht);
  ioopm_list_t *all_values = ioopm_hash_table_values(ht);
  
  bool result = false;
  for (size_t i = 0; i < ht_size && !result; ++i) {
    result = value_cmp(ioopm_linked_list_get(all_values, i), value);
  }
  
  ioopm_linked_list_destroy(all_values);
  return result;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  size_t ht_size = ioopm_hash_table_size(ht);
  ioopm_list_t *all_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *all_values = ioopm_hash_table_values(ht);
  
  bool result = true;
  for (size_t i = 0; i < ht_size && result; ++i) {
    result = pred(ioopm_linked_list_get(all_keys, i), ioopm_linked_list_get(all_values, i), arg);
  }

  ioopm_linked_list_destroy(all_keys);
  ioopm_linked_list_destroy(all_values);
  return result;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  size_t ht_size = ioopm_hash_table_size(ht);
  ioopm_list_t *all_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *all_values = ioopm_hash_table_values(ht);
  
  bool result = false;
  for (size_t i = 0; i < ht_size && !result; ++i) {
    result = pred(ioopm_linked_list_get(all_keys, i), ioopm_linked_list_get(all_values, i), arg);
  }
  
  ioopm_linked_list_destroy(all_keys);
  ioopm_linked_list_destroy(all_values);
  return result;
}

///DODGE: can only modify elem values of type char* by strcpy, or void*
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{ 
  size_t ht_size = ioopm_hash_table_size(ht);
  ioopm_list_t *all_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *all_values = ioopm_hash_table_values(ht);
  
  for (size_t i = 0; i < ht_size; ++i) {
    apply_fun(ioopm_linked_list_get(all_keys, i), ioopm_linked_list_get(all_values, i), arg);
  }
  
  ioopm_linked_list_destroy(all_keys);
  ioopm_linked_list_destroy(all_values);
}
