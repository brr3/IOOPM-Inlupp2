#include <stdbool.h>
#include <assert.h>
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

static bool cmp_string(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
}

static void print()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create_custom(hash_string, cmp_string, 17, 0.9);
  add_merch_to_db(ht);
  //add_merch_to_db(ht);
  elem_t found_element1 = {.v = NULL};
  //elem_t found_element2 = {.v = NULL};
  elem_t elem_name1 = {.s = "Äpple"};
  //elem_t elem_name2 = {.s = "Päron"};
  assert(ioopm_hash_table_lookup(ht, elem_name1, &found_element1));
  //assert(ioopm_hash_table_lookup(ht, elem_name2, &found_element2));
  item_t item1 = *((item_t*) found_element1.v);
  //item_t item2 = *((item_t*) found_element2.v);
  printf("item1 name = %s\n", item1.name);
  printf("item1 desc = %s\n", item1.desc);
  printf("item1 price = %d\n", item1.price);
  //printf("item2 name = %s\n", item2.name);
  //printf("item2 desc = %s\n", item2.desc);
  //printf("item2 price = %d\n", item2.price);
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
