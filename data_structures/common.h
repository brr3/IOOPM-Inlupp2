#pragma once

#include <stdbool.h>
//#include "dbg.h"

typedef union elem elem_t;

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *v;
  char *s;
};

typedef int (*ioopm_hash_function)(elem_t t);
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
/// DODGE: sending in elem, not pointer to it - thus only able to change elem of type char* or void*
typedef void (*ioopm_apply_function)(elem_t key, elem_t value, void *extra);
