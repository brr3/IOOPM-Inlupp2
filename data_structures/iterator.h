#pragma once
#include "linked_list.h"

/// @brief Create an iterator for a given list
/// @param list | the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief Checks if there are more elements to iterate over
/// @param iter | the iterator
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one ste
/// @param iter | the iterator
/// @return the next element, or NULL if there is no next element
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);

/// @brief Remove the current element from the underlying list
/// @param iter | the iterator
/// @return the removed element, or NULL if there is no element to remove
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter);

/// @brief Insert a new element into the underlying list making the current element its next
/// @param iter    | the iterator
/// @param element | the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter | the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter | the iterator
/// @return the current element, or NULL if no element was found
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter | the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);
