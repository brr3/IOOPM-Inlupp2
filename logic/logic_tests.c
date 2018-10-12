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


static void print()
{
  item_t item = make_merch("Äpple", "En frukt", 100, "A1");
  print_item(item);
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
