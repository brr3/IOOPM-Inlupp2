#include <stdbool.h>
#include <assert.h>
#include "CUnit/Basic.h"
#include "logic/logic.h"
#include "logic/elem.h"
#include "user_interface/ui.h"

static int init_suite(void)
{
  return 0;
}

static int clean_suite(void)
{
  return 0;
}
//----------------------------------------------------------tests

void test_is_shelf(){
  char *shelf1 = "A11";
  char *shelf2 = "11";
  char *shelf3 = "AAAAAAAA";
  char *shelf4 = "";

  CU_ASSERT_TRUE(is_shelf(shelf1));
  CU_ASSERT_FALSE(is_shelf(shelf2));
  CU_ASSERT_FALSE(is_shelf(shelf3));
  CU_ASSERT_FALSE(is_shelf(shelf4));
}

void test_is_menu_key(){
  char *key1 = "A";
  char *key2 = "a";
  char *key3 = "w";
  char *key4 = "8";
  char *key5 = "";

  CU_ASSERT_TRUE(is_menu_key(key1));
  CU_ASSERT_TRUE(is_menu_key(key2));
  CU_ASSERT_FALSE(is_menu_key(key3));
  CU_ASSERT_FALSE(is_menu_key(key4));
  CU_ASSERT_FALSE(is_menu_key(key5));
}

void test_is_yn_key(){
  char *key1 = "Y";
  char *key2 = "";
  char *key3 = "No";
  char *key4 = "Bro";

  CU_ASSERT_TRUE(is_yn_key(key1));
  CU_ASSERT_FALSE(is_yn_key(key2));
  CU_ASSERT_TRUE(is_yn_key(key3));
  CU_ASSERT_FALSE(is_yn_key(key4));
}

void test_item_exists(){
  /* char *name1 = "sak"; */
  /* char *desc1 = "pryl"; */
  /* int price1 = 1337; */

  //storage_t *storage = make_storage();
  item_t *item1 = make_item("sak", "pryl", 1337);
  add_item_to_storage(storage, item1);

  CU_ASSERT_TRUE(item_exists(storage, "SAK"));

  destroy_storage(storage);
}

/* void test_make_item(){ */
/* } */

/* void test_make_shelf(){ */
/* } */

/* void test_find_shelf_in_item_shelves(){ */
/* } */

/* void test_add_item_to_storage(){ */
/* } */

/* void test_remove_item_from_storage(){ */
/* } */

/* void test_ */
//---------------------------------------------------------

int main(void)
{
  CU_pSuite pSuiteNW = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuiteNW = CU_add_suite("nextWord Suite", init_suite, clean_suite);
  
  if (NULL == pSuiteNW) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  if (
      (NULL == CU_add_test(pSuiteNW, "test_is_shelf", test_is_shelf)) ||
      (NULL == CU_add_test(pSuiteNW, "test_is_menu_key", test_is_menu_key)) ||
      (NULL == CU_add_test(pSuiteNW, "test_is_yn_key", test_is_yn_key)) ||
      (NULL == CU_add_test(pSuiteNW, "test_item_exists", test_item_exists))
     )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
