#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"

typedef struct list ioopm_list_t;
typedef struct iter ioopm_list_iterator_t;

#define nodes 10

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create();

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list | the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list    | the linked list that will be appended
/// @param element | the element to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t element);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list    | the linked list that will be prepended
/// @param element | the element to be prepended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t element);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list    | the linked list that will be extended
/// @param index   | the position in the list
/// @param element | the element to be inserted
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t element);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list  | the linked list that will be reducted
/// @param index | the position in the list
/// @return the element that was removed, or an empty elem_t if there is no element at given index
elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list  | the linked list that will be extended
/// @param index | the position in the list
/// @return the element at given index, or an empty elem_t if there is no element at given index
elem_t ioopm_linked_list_get(ioopm_list_t *list, int index);

/// @brief Test if an element is in the list
/// @param list    | the linked list
/// @param element | the element sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, __compar_fn_t compare_fun);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list | the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list | the linked list
/// @return true if the number of elements in the list is 0, else false
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list | the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list | the linked list
/// @param prop | the property to be tested
/// @param x    | an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, bool (*prop)(elem_t, void *), void *x);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list | the linked list
/// @param prop | the property to be tested
/// @param x    | an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, bool (*prop)(elem_t, void *), void *x);

/// @brief Apply a supplied function to all elements in a list
/// @param list | the linked list
/// @param fun  | the function to be applied
/// @param x    | an additional argument (may be NULL) that will be passed to all internal calls of fun
void ioopm_linked_apply_to_all(ioopm_list_t *list, void (*fun)(elem_t, void *), void *x);
