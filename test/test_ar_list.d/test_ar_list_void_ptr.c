#include "ar_list.c"
#include "c_lists/cll_error.h"
#include "common/cll_def.h"

// Test framework
#include "../interfaces.h"
#include "mock_cll_std_lib_interface.h"
#include <unity.h>

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
size_t list_memory_mock_size = 0, array_memory_mock_size = 0;
const size_t default_capacity = 10;
void *list_memory_mock = NULL, *array_memory_mock = NULL;
int arl_small_values[] = {0, 1, 2, 3, 4, 5};
size_t arl_small_length = sizeof(arl_small_values) / sizeof(int);
size_t FREE_COUNTER = 0;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  FREE_COUNTER = 0;

  array_memory_mock_size = sizeof(void *) * 10;

  array_memory_mock = malloc(array_memory_mock_size);
  if (!array_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory for `array_memory_mock`. "
                      "Mocking malloc failed!");

  list_memory_mock_size = sizeof(struct ar_list);

  list_memory_mock = malloc(list_memory_mock_size);
  if (!list_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory for `list_memory_mock`. "
                      "Mocking malloc failed!");
}

void tearDown(void) {

  free(array_memory_mock);

  free(list_memory_mock);

  array_memory_mock = NULL;

  list_memory_mock = NULL;
}

/*******************************************************************************
 *    TESTS UTILS
 ******************************************************************************/
void TEST_ASSERT_EQUAL_ERROR(cll_error expected, cll_error received) {
  TEST_ASSERT_EQUAL_STRING(cll_strerror(expected), cll_strerror(received));
}

arl_ptr setup_empty_list() {
  arl_ptr l;
  cll_error err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);

  return l;
}

arl_ptr setup_small_list() {
  arl_ptr l = setup_empty_list();
  size_t i;

  for (i = 0; i < arl_small_length; i++) {
    l->array[i] = &arl_small_values[i];
    l->length += 1;
  }

  return l;
}

void parametrize_test_arl_get_i_too_big_failure(arl_ptr l);
void parametrize_test_arl_set_i_too_big_failure(arl_ptr l);

void test_arl_create_memory_failure_array(void) {
  arl_ptr l;
  cll_error err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, NULL);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OUT_OF_MEMORY, err);
}

void test_arl_create_memory_failure_list(void) {
  arl_ptr l;
  cll_error err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, NULL);
  app_free_Expect(array_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OUT_OF_MEMORY, err);
}

void test_arl_create_success(void) {

  arl_ptr l;
  cll_error err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(l->length, 0);
  TEST_ASSERT_EQUAL(l->capacity, default_capacity);
}

void test_arl_get_i_too_big_failure(void) {
  arl_ptr ls_to_param[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ls_to_param) / sizeof(arl_ptr); i++)
    parametrize_test_arl_get_i_too_big_failure(ls_to_param[i]);
}

void parametrize_test_arl_get_i_too_big_failure(arl_ptr l) {
  size_t i, indexes_to_check[] = {l->length, l->length + 1, l->length + 2};
  void *c;
  cll_error err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_get(l, indexes_to_check[i], &c);

    TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_get_success(void) {
  arl_ptr l = setup_small_list();
  size_t i;
  void *c;
  cll_error err;

  for (i = 0; i < l->length; i++) {
    err = arl_get(l, i, &c);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL(&arl_small_values[i], c);
  }
}
void test_arl_set_i_too_big_failure(void) {
  arl_ptr ls_to_param[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ls_to_param) / sizeof(arl_ptr); i++)
    parametrize_test_arl_set_i_too_big_failure(ls_to_param[i]);
}

void parametrize_test_arl_set_i_too_big_failure(arl_ptr l) {
  size_t i, indexes_to_check[] = {l->length, l->length + 1, l->length + 2};
  char value[] = "ABC";
  cll_error err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_set(l, indexes_to_check[i], value);
    TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_set_success(void) {
  arl_ptr l = setup_small_list();
  int i, value = 13;
  cll_error err;

  for (i = 0; i < arl_small_length; i++) {
    err = arl_set(l, i, (void *)&value);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL_PTR(&value, l->array[i]);
    TEST_ASSERT_EQUAL(value, *(int *)(l->array[i]));
  }
}

void test_arl_slice_start_i_too_big(void) {
  size_t slice_len;
  cll_error err;
  int i;
  void *c;
  arl_ptr l = setup_small_list();

  slice_len = 0;

  i = l->length;

  err = arl_slice(l, i, slice_len, &c);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INDEX_TOO_BIG, err);
}

void test_arl_slice_too_much_elements_to_move(void) {
  size_t slice_len;
  cll_error err;
  int i;
  void *c;
  arl_ptr l = setup_small_list();

  slice_len = l->length;

  i = 0;

  err = arl_slice(l, i, slice_len, &c);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INVALID_ARGS, err);
}

void test_arl_slice_success(void) {
  size_t slice_len;
  cll_error err;
  int i;
  arl_ptr l = setup_small_list();

  slice_len = 3;
  void *slice[slice_len];

  void *expected[] = {l->array[1], l->array[2], l->array[3]};

  i = 1;

  err = arl_slice(l, i, slice_len, slice);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, slice, slice_len);
}

void test_arl_move_elements_left_new_length_underflow_failure(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_left(l, l->length + 1, l->length + 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_left_elements_to_move_underflow_failure(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_left(l, l->length + 1, 0);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_left_delete_index_0(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[1], l->array[2], l->array[3], l->array[4],
                      l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 1, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_index_1(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[2], l->array[3],
                      l->array[4], l->array[5], NULL};
  cll_error err;

  err = arl_move_elements_left(l, 2, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_index_2(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[3], l->array[4],
                      l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 3, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_index_3(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], l->array[4],
                      l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 4, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_index_4(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], l->array[3],
                      l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 5, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_index_5(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], l->array[3],
                      l->array[4]};
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(5, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 5);
}

void test_arl_move_elements_left_delete_last_indexes_0(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], l->array[3]};
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 2);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(4, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_last_indexes_1(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], NULL, NULL, NULL};
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 3);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(3, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, 3);
}

void test_arl_move_elements_left_delete_all(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 999);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(0, l->length);
}

void test_arl_move_elements_left_move_by_bigger_than_start_i(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[4], l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 4, 5);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, l->length);
}

/* Confirms:
 *    INPUT  l.array {0, 1, 2, 3, 4, 5, , , , }, start_i 3, move_by 2
 *    OUTPUT l.array {0, 3, 4, 5, , , , , , }
 */
void test_arl_move_elements_left_success(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[3], l->array[4], l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 3, 2);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, l->length);
}

/* void test_arl_create_overflow_failure(void) { */
/*   arl_ptr l; */
/*   cll_error err; */

/*   err = arl_create(&l, default_capacity); */

/*   TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OVERFLOW, err); */
/* } */
