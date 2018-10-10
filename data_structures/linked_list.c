#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


typedef struct node node_t;

struct list
{
  node_t *first;
  node_t *last;
  size_t size;
};

struct node
{
  elem_t data;
  node_t *next;
};

struct iter 
{
  node_t *current;
  ioopm_list_t *list;
};


ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list) 
{
  ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));
  result->current = list->first;
  result->list = list;
  return result;
}


static node_t *link_new(elem_t value, node_t *next)
{
  node_t *new_node = calloc(1, sizeof(node_t));
  if (new_node)
    {
      new_node->data = value;
      new_node->next = next;
    }
  return new_node;
}


bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  return iter->current->next != NULL; 
}


elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if (ioopm_iterator_has_next(iter))
    {
      iter->current = iter->current->next;
      return iter->current->data;
    }
  else
    {
      return (elem_t) {.void_ptr = NULL};
    }
}


elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  if (ioopm_iterator_has_next(iter))
    {
      node_t *to_remove = iter->current->next;
      elem_t result;
      result = to_remove->data;
      
      iter->current->next = to_remove->next; 
      iter->list->size -= 1;
      free(to_remove);
      
      return result;
    }
  else
    {
      return (elem_t) {.void_ptr = NULL};
    }
}


void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t data)
{
  iter->current->next = link_new(data, iter->current->next);
  iter->list->size += 1;
}


void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter->current = iter->list->first;
}


elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  if (ioopm_iterator_has_next(iter))
    {
      return iter->current->next->data; 
    }
  else
    {
      return (elem_t) {.void_ptr = NULL};
    }
}


void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
}


/*elem_t iterator_remove(ioopm_list_iterator_t *iter) SEG FAULT, REDOVISNING R52
{
  node_t *to_remove = iter->current->next; /// Cache result
  elem_t result = to_remove->data;
  
  iter->current->next = to_remove->next;  /// Move forward
  free(to_remove); /// Remove link
  return result;
} */ 


ioopm_list_t *ioopm_linked_list_create()
{
  ioopm_list_t *new_list = calloc(1, sizeof(ioopm_list_t));
  if (new_list)
    {
      // Use of calloc will ensure result->first->next == NULL
      new_list->first = new_list->last = calloc(1, sizeof(node_t));
      new_list->size = 0;
    }
  return new_list;
}


void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_linked_list_clear(list); 
  free(list->first); // Free the sentinel
  free(list);
}


void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  ioopm_linked_list_insert(list, ioopm_linked_list_size(list), value);
}


void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  ioopm_linked_list_insert(list, 0, value);
}


static node_t *list_inner_find_previous(node_t *first_node, int index)
{
  node_t *previous_node = first_node;
  for (int i = 0; i < index; ++i)
    {
      previous_node = previous_node->next;
    }
  return previous_node;
}


static int list_inner_adjust_index(int index, int upper_bound)
{
  return abs(index) % upper_bound;
}


void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
  int list_size = ioopm_linked_list_size(list);
  int valid_index = list_inner_adjust_index(index, list_size + 1);
  node_t *previous_node;
  
  if (valid_index < list_size)
    {
      previous_node = list_inner_find_previous(list->first, valid_index);  
    }
  else
    {
      previous_node = list_inner_find_previous(list->last, valid_index);
    }  
  previous_node->next = link_new(value, previous_node->next);
  list->size += 1;
}


elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  int list_size = ioopm_linked_list_size(list);
  int valid_index = list_inner_adjust_index(index, list_size);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  
  for (int i = 0; i < valid_index; ++i)
    {
      ioopm_iterator_next(iter); 
    }
  elem_t removed_data = ioopm_iterator_remove(iter);
  ioopm_iterator_destroy(iter);

  return removed_data;
}


elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  int list_size = ioopm_linked_list_size(list);
  int valid_index = list_inner_adjust_index(index, list_size);  
  node_t *previous_node = list_inner_find_previous(list->first, valid_index);

  if (previous_node != NULL)
    {
      return previous_node->next->data;
    }
  else
    {
      return (elem_t) {.void_ptr = NULL};
    }
}


bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t value, cmp_fun_t compare_fun)
{
  for (node_t *cursor = list->first->next; cursor != NULL; cursor = cursor->next)
    {
      if (compare_fun(cursor->data, value) == 0) return true;
    }
  return false;
}


size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}


bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return list->size == 0;
}


void ioopm_linked_list_clear(ioopm_list_t *list)
{
  while (ioopm_linked_list_size(list) > 0)
    {
      ioopm_linked_list_remove(list, 0);
    }
}


bool ioopm_linked_list_all(ioopm_list_t *list, bool (*prop)(elem_t, void *), void *x)
{
  node_t *cursor = list->first->next;
  bool result = true;
  while(cursor != NULL && result)
    {
      result = prop(cursor->data, x);
      cursor = cursor->next;
    }
  return result;
}


bool ioopm_linked_list_any(ioopm_list_t *list, bool (*prop)(elem_t, void *), void *x)
{
  node_t *cursor = list->first->next;
  bool result = false;
  while(cursor != NULL && !result)
    {
      result = prop(cursor->data, x);
      cursor = cursor->next;
    }
  return result;
}


void ioopm_linked_apply_to_all(ioopm_list_t *list, void (*fun)(elem_t, void *), void *x)
{
  for (node_t *cursor = list->first->next; cursor != NULL; cursor = cursor->next)
    {
      fun(cursor->data, x);
    }
}
