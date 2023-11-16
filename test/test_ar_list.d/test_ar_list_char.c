/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>
#include <stdint.h>

// App
#include "ar_list.c"
#include "c_lists/cll_def.h"
#include "c_lists/cll_error.h"

// Test framework
#include "../interfaces.h"
#include "mock_cll_std_lib_interface.h"
#include <unity.h>

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
char arl_small_values[] = {'d', 'u', 'n', 'e', 0};
size_t arl_small_length = sizeof(arl_small_values) / sizeof(char);
const size_t default_capacity = 10;
void *list_memory_mock = NULL;
char *array_memory_mock = NULL;
size_t list_memory_mock_size = 0, array_memory_mock_size = 0, free_counter = 0;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  free_counter = 0;

  array_memory_mock_size = sizeof(char) * default_capacity;

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
void parametrize_test_arl_get_i_too_big_failure(arl_ptr l);
void parametrize_test_arl_set_i_too_big_failure(arl_ptr l);

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
    l->array[i] = arl_small_values[i];
    l->length += 1;
  }

  return l;
}

void mock_app_realloc(arl_ptr l, size_t new_array_size) {
  // Do not use realloc, it would invalidate l->array
  // before actual function execution.
  void *new_array;

  new_array = malloc(new_array_size);
  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to allocate memory for realloc mock!");

  new_array = memcpy(new_array, l->array, l->capacity * CLL_PTR_SIZE);

  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to copy memory for realloc mock!");

  free(array_memory_mock);

  array_memory_mock = new_array;

  app_realloc_ExpectAndReturn(l->array, new_array_size, new_array);
}

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

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
  char c;
  cll_error err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_get(l, indexes_to_check[i], &c);

    TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_get_success(void) {
  arl_ptr l = setup_small_list();
  size_t i;
  char c;
  cll_error err;

  for (i = 0; i < l->length; i++) {
    err = arl_get(l, i, &c);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL(arl_small_values[i], c);
  }
}

void test_arl_slice_start_i_too_big(void) {
  size_t slice_len;
  cll_error err;
  int i;
  char c;
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
  char c;
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
  char slice[slice_len];

  char expected[] = {l->array[1], l->array[2], l->array[3]};

  i = 1;

  err = arl_slice(l, i, slice_len, slice);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, slice, slice_len);
}

void test_arl_set_i_too_big_failure(void) {
  arl_ptr ls_to_param[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ls_to_param) / sizeof(arl_ptr); i++)
    parametrize_test_arl_set_i_too_big_failure(ls_to_param[i]);
}

void parametrize_test_arl_set_i_too_big_failure(arl_ptr l) {
  size_t i, indexes_to_check[] = {l->length, l->length + 1, l->length + 2};
  char value = 'A';
  cll_error err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_set(l, indexes_to_check[i], value);
    TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_set_success(void) {
  arl_ptr l = setup_small_list();
  int i;
  char value = 'A';
  cll_error err;

  for (i = 0; i < arl_small_length; i++) {
    err = arl_set(l, i, value);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL(value, l->array[i]);
  }
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/

void test_arl_move_elements_right_new_length_overflow_failure(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_right(l, 0, CLL_SIZE_T_MAX);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OVERFLOW, err);
}

void test_arl_move_elements_right_new_length_invalid(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_right(l, 0, l->capacity + 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_INVALID_ARGS, err);
}

void test_arl_move_elements_right_elements_to_move_amount_underflow_failure(
    void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_right(l, l->length + 1, 0);

  TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_right_no_elements_to_move(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  err = arl_move_elements_right(l, l->length, 0);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
}

/* Confirms:
 *    INPUT  l.array {0, 1, 2, 3, 4, 5, , , , }, start_i 1, move_by 2
 *    OUTPUT l.array {0, , , 1, 2, 3, 4, 5, , }
 */
void test_arl_move_elements_right_success(void) {
  int null_indexes[] = {1, 2, 3};
  size_t null_i_length = sizeof(null_indexes) / sizeof(int);

  arl_ptr l = setup_small_list();
  size_t i;
  char value;
  cll_error err;

  err = arl_move_elements_right(l, 1, 3);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(8, l->length);

  // Values before start_i should'n be changed
  for (i = 0; i < null_indexes[0]; i++) {
    err = arl_get(l, i, &value);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL(arl_small_values[i], value);
  }

  // Old values are moved by move_by
  int last_null_index = null_indexes[null_i_length - 1];

  for (i = last_null_index + 1; i < l->length; i++) {
    err = arl_get(l, i, &value);

    TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
    TEST_ASSERT_EQUAL(arl_small_values[i - last_null_index], value);
  }
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
  char expected[] = {
      l->array[1],
      l->array[2],
      l->array[3],
      l->array[4],
  };
  cll_error err;

  err = arl_move_elements_left(l, 1, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_index_1(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {
      l->array[0],
      l->array[2],
      l->array[3],
      l->array[4],
  };
  cll_error err;

  err = arl_move_elements_left(l, 2, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_index_2(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {
      l->array[0],
      l->array[1],
      l->array[3],
      l->array[4],
  };
  cll_error err;

  err = arl_move_elements_left(l, 3, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_index_3(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[0], l->array[1], l->array[2], l->array[4]};
  cll_error err;

  err = arl_move_elements_left(l, 4, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_index_4(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[0], l->array[1], l->array[2], l->array[3]};
  cll_error err;

  err = arl_move_elements_left(l, 5, 1);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, 4);
}

void test_arl_move_elements_left_delete_last_indexes_0(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[0], l->array[1], l->array[2], l->array[3]};
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 5, 2);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(3, l->length);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, l->length);
}

void test_arl_move_elements_left_delete_last_indexes_1(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[0], l->array[1], l->array[2]};
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 5, 3);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(2, l->length);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, l->length);
}

void test_arl_move_elements_left_delete_all(void) {
  arl_ptr l = setup_small_list();
  cll_error err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 5, 999);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL(0, l->length);
}

void test_arl_move_elements_left_move_by_bigger_than_start_i(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[3], l->array[4]};
  cll_error err;

  err = arl_move_elements_left(l, 3, 4);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, l->length);
}

/* Confirms:
 *    INPUT  l.array {0, 1, 2, 3, 4, 5, , , , }, start_i 3, move_by 2
 *    OUTPUT l.array {0, 3, 4, 5, , , , , , }
 */
void test_arl_move_elements_left_success(void) {
  arl_ptr l = setup_small_list();
  char expected[] = {l->array[0], l->array[3], l->array[4], l->array[5]};
  cll_error err;

  err = arl_move_elements_left(l, 3, 2);

  TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, l->array, l->length);
}

/* void test_arl_grow_array_capacity_memory_failure(void) { */
/*   arl_ptr l = setup_small_list(); */
/*   cll_error err; */

/*   app_realloc_IgnoreAndReturn(NULL); */

/*   err = arl_grow_array_capacity(l); */

/*   TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OUT_OF_MEMORY, err); */
/*   TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array); */
/*   TEST_ASSERT_EQUAL(arl_small_length, l->length); */
/*   TEST_ASSERT_EQUAL(array_memory_mock_size / sizeof(char), l->capacity); */
/* } */

/* void test_arl_grow_array_capacity_max_failure(void) { */
/*   arl_ptr l = setup_small_list(); */
/*   cll_error err; */

/*   l->capacity = CLL_SIZE_T_MAX; */

/*   err = arl_grow_array_capacity(l); */

/*   TEST_ASSERT_EQUAL_ERROR(CLL_ERROR_OVERFLOW, err); */
/* } */

/* void test_arl_grow_array_capacity_success(void) { */
/*   arl_ptr l = setup_small_list(); */
/*   size_t new_l_capacity, new_array_size; */
/*   cll_error err; */

/*   err = arl_count_new_capacity(l->length, l->capacity, &new_l_capacity); */

/*   TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err); */

/*   new_array_size = new_l_capacity * sizeof(char); */

/*   mock_app_realloc(l, new_array_size); */

/*   err = arl_grow_array_capacity(l); */

/*   TEST_ASSERT_EQUAL_ERROR(CLL_SUCCESS, err); */
/*   TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array); */
/*   TEST_ASSERT_EQUAL(new_l_capacity, l->capacity); */
/* } */
