#include "ar_list.c"
#include "c_lists/cll_def.h"
#include "c_lists/cll_error.h"
#include "common/cll_def.h"

// Test framework
#include "../interfaces.h"
#include "mock_cll_std_lib_interface.h"
#include <stddef.h>
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

void parametrize_test_arl_get_i_too_big_failure(arl_ptr l);
void parametrize_test_arl_set_i_too_big_failure(arl_ptr l);

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
