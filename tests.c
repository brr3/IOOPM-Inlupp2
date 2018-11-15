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

void test_make_item(){
  
  storage_t *storage = make_storage();
  item_t *item = make_item(strdup("name"), strdup("desc"), 1337);

  add_item_to_storage(storage, item);

  CU_ASSERT_TRUE(item_exists(*storage, strdup("name")));

  destroy_storage(storage);
}

void test_remove_item_from_storage(){
  
  storage_t *storage = make_storage();
  item_t *item = make_item(strdup("name"), strdup("desc"), 1337);
  add_item_to_storage(storage, item);
  
  remove_item_from_storage(storage, item);

  CU_ASSERT_FALSE(item_exists(*storage, strdup("name")));

  destroy_storage(storage);
}

void test_item_names_to_sorted_array(){

  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  char *arr[4];
  item_names_to_sorted_array(*storage, arr);

  CU_ASSERT_TRUE(0 == strcmp(arr[0], "NAME1"));

  destroy_storage(storage);
}

void test_extract_item_from_storage(){
  
  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  char *tmp = strdup("name1");
  elem_t found_value = {.v = item1};
  item_t *extracted_item = extract_item_from_storage(*storage, tmp, &found_value);

  puts(extracted_item->name);
  CU_ASSERT_TRUE(0 == strcmp(strdup("NAME1"), extracted_item->name));
  
  destroy_storage(storage);
}

void test_location_exists(){

  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  add_shelf_to_storage(storage, item1, strdup("A01"), 10);
  add_shelf_to_storage(storage, item2, strdup("A02"), 9);
  add_shelf_to_storage(storage, item3, strdup("A03"), 11);
  add_shelf_to_storage(storage, item4, strdup("A04"), 12);

  CU_ASSERT_TRUE(location_exists(*storage, strdup("A01"), NULL));
  CU_ASSERT_TRUE(location_exists(*storage, strdup("A02"), NULL));
  CU_ASSERT_FALSE(location_exists(*storage, strdup("A05"), NULL));
  CU_ASSERT_FALSE(location_exists(*storage, strdup(""), NULL));
  
  destroy_storage(storage);
}

void test_find_shelf_in_item(){
  
  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  add_shelf_to_storage(storage, item1, strdup("A01"), 10);
  add_shelf_to_storage(storage, item2, strdup("A02"), 9);
  add_shelf_to_storage(storage, item3, strdup("A03"), 11);
  add_shelf_to_storage(storage, item4, strdup("A04"), 12);

  shelf_t *found_shelf = find_shelf_in_item(item1, "A01");

  CU_ASSERT_TRUE(0 == strcmp(strdup(found_shelf->shelf_name), strdup("A01")));
  
  destroy_storage(storage);
}

void test_checkout_cart_item(){
  
  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  add_shelf_to_storage(storage, item1, strdup("A01"), 10);
  add_shelf_to_storage(storage, item2, strdup("A02"), 9);
  add_shelf_to_storage(storage, item3, strdup("A03"), 11);
  add_shelf_to_storage(storage, item4, strdup("A04"), 12);

  add_cart_to_storage(storage);
  add_item_to_cart(*storage, *item1, 3, 1);
  add_item_to_cart(*storage, *item2, 1, 1);
  cart_t *cart = extract_cart_from_storage(*storage, 1);

  checkout_cart_items(storage, cart);
  
  destroy_storage(storage);
}

void test_cart_exists(){

  storage_t *storage = make_storage();
  add_cart_to_storage(storage);

  CU_ASSERT_TRUE(cart_exists(*storage, 1));
  CU_ASSERT_FALSE(cart_exists(*storage, 2));

  destroy_storage(storage);
}

void test_remove_cart_from_storage(){
  
  storage_t *storage = make_storage();
  add_cart_to_storage(storage);

  CU_ASSERT_TRUE(cart_exists(*storage, 1));

  remove_cart_from_storage(storage, 1);

  CU_ASSERT_FALSE(cart_exists(*storage, 1));

  destroy_storage(storage);
}

void test_remove_item_from_cart(){

  storage_t *storage = make_storage();
  item_t *item1 = make_item(strdup("name1"), strdup("desc1"), 1337);
  item_t *item2 = make_item(strdup("name2"), strdup("desc2"), 1338);
  item_t *item3 = make_item(strdup("name3"), strdup("desc3"), 1339);
  item_t *item4 = make_item(strdup("name4"), strdup("desc4"), 1340);
  add_item_to_storage(storage, item1);
  add_item_to_storage(storage, item2);
  add_item_to_storage(storage, item3);
  add_item_to_storage(storage, item4);

  add_shelf_to_storage(storage, item1, strdup("A01"), 10);
  add_shelf_to_storage(storage, item2, strdup("A02"), 9);
  add_shelf_to_storage(storage, item3, strdup("A03"), 11);
  add_shelf_to_storage(storage, item4, strdup("A04"), 12);

  add_cart_to_storage(storage);
  add_item_to_cart(*storage, *item1, 3, 1);
  add_item_to_cart(*storage, *item2, 1, 1);

  cart_t *cart = extract_cart_from_storage(*storage, 1);

  remove_item_from_cart(cart, 1);
  remove_item_from_cart(cart, 2);

  destroy_storage(storage);
}

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
      (NULL == CU_add_test(pSuiteNW, "test_make_item", test_make_item)) ||
      (NULL == CU_add_test(pSuiteNW, "test_remove_item_from_storage", test_remove_item_from_storage)) ||
      (NULL == CU_add_test(pSuiteNW, "test_item_names_to_sorted_array", test_item_names_to_sorted_array)) ||
      (NULL == CU_add_test(pSuiteNW, "test_location_exists", test_location_exists)) ||
      (NULL == CU_add_test(pSuiteNW, "test_find_shelf_in_item", test_find_shelf_in_item)) ||
      (NULL == CU_add_test(pSuiteNW, "test_checkout_cart_item", test_checkout_cart_item)) ||
      (NULL == CU_add_test(pSuiteNW, "test_extract_item_from_storage", test_extract_item_from_storage)) ||
      (NULL == CU_add_test(pSuiteNW, "test_cart_exists", test_cart_exists)) ||
      (NULL == CU_add_test(pSuiteNW, "test_remove_cart_from_storage", test_remove_cart_from_storage)) ||
      (NULL == CU_add_test(pSuiteNW, "test_remove_item_from_cart", test_remove_item_from_cart))
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
