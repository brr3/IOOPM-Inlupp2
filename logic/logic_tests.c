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
  add_merch_to_db(ht);
  puts("add merch done");
  //add_merch_to_db(ht);
  elem_t found_element;
  elem_t elem_name = {.s = "Äpple"};
  puts("before lookup");
  ioopm_hash_table_lookup(ht, elem_name, &found_element);
  puts("after lookup");
  item_t item1 = *((item_t*) found_element.v);
  //item_t item2 = make_merch("Päron", "En annan frukt", 50);
  
  print_item(item1);
  //print_item(item2);
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
