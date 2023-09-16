/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Test framework
#include "ar_list.h"
#include "mock_std_lib_utils.h"
#include <unity.h>

// App
/* Including source file instead of header file allows
 *  for static functions testing.
 *  Do not forget adding `-zmuldefs` to gcc.*/
#include "ar_list.c"
#include "l_def.h"
#include "l_error.h"

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
size_t list_memory_mock_size = 0, array_memory_mock_size = 0;
void *list_memory_mock = NULL, *array_memory_mock = NULL;
int arl_small_values[] = {0, 1, 2, 3, 4, 5};
size_t arl_small_length = sizeof(arl_small_values) / sizeof(int);

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  array_memory_mock_size = L_PTR_SIZE * 10;

  array_memory_mock = malloc(array_memory_mock_size);
  if (!array_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory. Mocking malloc failed!");

  list_memory_mock_size = sizeof(struct ar_list);

  list_memory_mock = malloc(list_memory_mock_size);
  if (!list_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory. Mocking malloc failed!");

  get_pointer_size_IgnoreAndReturn(L_PTR_SIZE);
}

void tearDown(void) {

  if (array_memory_mock)
    free(array_memory_mock);

  if (list_memory_mock)
    free(list_memory_mock);

  array_memory_mock = NULL;
  array_memory_mock = 0;

  list_memory_mock = NULL;
  list_memory_mock = 0;
}

/*******************************************************************************
 *    TESTS UTILS
 ******************************************************************************/

arl_ptr setup_empty_list() {
  arl_ptr l;
  size_t default_capacity = array_memory_mock_size / L_PTR_SIZE;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  arl_create(&l, default_capacity);

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
/* /\*******************************************************************************
 */
/*  *    PUBLIC API TESTS */
/*  ******************************************************************************\/
 */

void test_arl_create_memory_failure_array(void) {
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, NULL);

  err = arl_create(&l, 10);

  TEST_ASSERT_EQUAL(L_ERROR_OUT_OF_MEMORY, err);
}
void test_arl_create_memory_failure_list(void) {
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, NULL);
  array_memory_mock = NULL;

  err = arl_create(&l, 10);

  TEST_ASSERT_EQUAL(L_ERROR_OUT_OF_MEMORY, err);
}

void test_arl_create_overflow_failure(void) {
  arl_ptr l;
  l_error_t err;

  err = arl_create(&l, ARL_CAPACITY_MAX + 1);

  TEST_ASSERT_EQUAL(L_ERROR_OVERFLOW, err);
}

void test_arl_create_success(void) {
  size_t default_capacity = array_memory_mock_size / sizeof(void *);
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL(L_SUCCESS, err);
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
  void *p;
  l_error_t err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_get(l, indexes_to_check[i], &p);
    TEST_ASSERT_EQUAL(L_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_get_success(void) {
  arl_ptr l = setup_small_list();
  int *value, i;
  l_error_t err;

  for (i = 0; i < l->length; i++) {
    err = arl_get(l, i, (void **)&value);

    TEST_ASSERT_EQUAL(L_SUCCESS, err);
    TEST_ASSERT_EQUAL(arl_small_values[i], *value);
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
  l_error_t err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_set(l, indexes_to_check[i], value);
    TEST_ASSERT_EQUAL(L_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_set_success(void) {
  arl_ptr l = setup_small_list();
  int value = 13;
  int i;
  l_error_t err;

  for (i = 0; i < arl_small_length; i++) {
    err = arl_set(l, i, (void *)&value);

    TEST_ASSERT_EQUAL(L_SUCCESS, err);
    TEST_ASSERT_EQUAL_PTR(&value, l->array[i]);
    TEST_ASSERT_EQUAL(13, *(int *)(l->array[i]));
  }
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/
void test_arl_count_new_capacity(void) {
  /* Show array capacity growth ratio by example. */
  /* Purpose of this function is mainly documentational. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t length = 0;
  size_t capacity = 1000;
  size_t i;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    capacity = length = arl_count_new_capacity(length, capacity);

    TEST_ASSERT_EQUAL(expected_values[i], capacity);
  }
}

void test_arl_count_new_capacity_overflow_failure(void) {
  /* Show array growth stop. */
  /* Upper bound of list is well defined. */
  size_t length, capacity;

  capacity = length = ARL_CAPACITY_MAX;

  capacity = arl_count_new_capacity(length, capacity);

  TEST_ASSERT_EQUAL(ARL_CAPACITY_MAX, capacity);
}

void test_arl_is_i_too_big_true(void) {
  arl_ptr l = setup_small_list();
  size_t j, i_to_check[] = {l->length, l->length + 1, l->capacity};
  bool is_i_too_big;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_i_too_big = arl_is_i_too_big(l, i_to_check[j]);

    TEST_ASSERT_TRUE(is_i_too_big);
  }
}

void test_arl_is_i_too_big_false(void) {
  arl_ptr l = setup_small_list();
  size_t j, i_to_check[] = {l->length - 1, 0};
  bool is_i_too_big;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_i_too_big = arl_is_i_too_big(l, i_to_check[j]);

    TEST_ASSERT_FALSE(is_i_too_big);
  }
}

void test_arl_grow_array_capacity_memory_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  app_realloc_IgnoreAndReturn(NULL);

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL(L_ERROR_OUT_OF_MEMORY, err);
  TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array);
  TEST_ASSERT_EQUAL(arl_small_length, l->length);
  TEST_ASSERT_EQUAL(array_memory_mock_size / sizeof(void *), l->capacity);
}

void test_arl_grow_array_capacity_max_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  l->capacity = ARL_CAPACITY_MAX;

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL(L_ERROR_REACHED_CAPACITY_MAX, err);
}

void test_arl_grow_array_capacity_success(void) {
  arl_ptr l = setup_small_list();
  size_t new_l_capacity, new_array_size;
  void *new_array;
  l_error_t err;

  new_l_capacity = arl_count_new_capacity(l->length, l->capacity);

  new_array_size = new_l_capacity * L_PTR_SIZE;

  // Do not use realloc, it would invalidate l->array before
  //    actual function execution.
  new_array = malloc(new_array_size);
  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to allocate memory for realloc mock!");

  new_array = memcpy(new_array, l->array, l->capacity * L_PTR_SIZE);

  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to copy memory for realloc mock!");

  array_memory_mock = new_array;

  app_realloc_ExpectAndReturn(l->array, new_array_size, new_array);

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(new_array, l->array);
  TEST_ASSERT_EQUAL(new_l_capacity, l->capacity);
}

/* void test_arl_move_indexes_by_positive_number_new_length_overflow_failure( */
/*     void) { */
/*   arl_ptr l = setup_small_list(); */
/*   l_error_t err; */

/*   err = arl_move_indexes_by_positive_number(l, 0, L_SIZE_T_MAX); */

/*   TEST_ASSERT_EQUAL(L_ERROR_OVERFLOW, err); */
/* } */

/* void
 * test_arl_move_indexes_by_positive_number_elements_to_move_amount_underflow_failure(
 */
/*     void) { */
/*   arl_ptr l = setup_small_list(); */
/*   l_error_t err; */

/*   err = arl_move_indexes_by_positive_number(l, l->length + 1, 0); */

/*   TEST_ASSERT_EQUAL(L_ERROR_OVERFLOW, err); */
/* } */

/* void test_arl_alloc_array */
