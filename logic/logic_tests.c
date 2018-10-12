#include <stdbool.h>
#include "CUnit/Basic.h"
#include "logic.h"
#include "elem.h"

static int init_suite(void)
{
  return 0;
}

static int clean_suite(void)
{
  return 0;
}


static int hash_string(elem_t key)
{
  char *str = key.s;
  int result = 0;
  do
    {
      result = result * 31 + *str;
    }
  while (*++str != '\0'); 
  return result;
}

static bool cmp_int(elem_t a, elem_t b)
{
  return a.i - b.i == 0;
}

static void print()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create_custom(hash_string, cmp_int, 17, 0.9);
  item_t item1 = make_merch("Äpple", "En frukt", 100, "A1");
  elem_t item1_name = {.s = item1.name};
  elem_t item1_ptr = {.v = &item1};
  ioopm_hash_table_insert(ht, item1_name, item1_ptr);
  
  item_t item2 = make_merch("Päron", "En annan frukt", 50, "H20");
  elem_t item2_name = {.s = item2.name};
  elem_t item2_ptr = {.v = &item2};
  ioopm_hash_table_insert(ht, item2_name, item2_ptr);

  elem_t found_element;
  ioopm_hash_table_lookup(ht, item1_name, &found_element);
  item_t found_item = *((item_t*) found_element.v);

  print_item(found_item);
  
  ioopm_hash_table_lookup(ht, item2_name, &found_element);
  found_item = *((item_t*) found_element.v);
  
  print_item(found_item); 
}


int main(void)
{
  print();
  /*
  CU_pSuite pSuiteNW = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuiteNW = CU_add_suite("nextWord Suite", init_suite, clean_suite);
  
  if (NULL == pSuiteNW) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  if (NULL == CU_add_test(pSuiteNW, "worthless test", print)
      ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error(); */
}
