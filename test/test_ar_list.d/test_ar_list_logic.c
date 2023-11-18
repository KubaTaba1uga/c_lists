/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// App
#include "arl_list.c"

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

  list_memory_mock_size = sizeof(struct arl_def);

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
void TEST_ASSERT_EQUAL_ERROR(arl_error expected, arl_error received) {
  TEST_ASSERT_EQUAL_STRING(arl_strerror(expected), arl_strerror(received));
}

arl_ptr setup_empty_list() {
  arl_ptr l;
  arl_error err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(ARL_SUCCESS, err);

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

void mock_app_realloc(arl_ptr l, size_t new_array_size) {
  // Do not use realloc, it would invalidate l->array
  // before actual function execution.
  void *new_array;

  new_array = malloc(new_array_size);
  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to allocate memory for realloc mock!");

  new_array = memcpy(new_array, l->array, l->capacity * sizeof(void *));

  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to copy memory for realloc mock!");

  free(array_memory_mock);

  array_memory_mock = new_array;

  app_realloc_ExpectAndReturn(l->array, new_array_size, new_array);
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/

void test__count_new_capacity(void) {
  /* Show array capacity growth ratio by example. */
  /* Purpose of this function is mainly documentational. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t length = 0;
  size_t capacity = 1000;
  size_t value;
  size_t i;
  arl_error err;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    err = _count_new_capacity(length, capacity, &value);

    capacity = length = value;

    TEST_ASSERT_EQUAL_ERROR(ARL_SUCCESS, err);
    TEST_ASSERT_EQUAL(expected_values[i], capacity);
  }
}

void test__count_new_capacity_overflow_failure(void) {
  /* Show array growth stop. */
  /* Upper bound of list is well defined. */
  size_t length, capacity, value;
  arl_error err;

  capacity = length = ARL_SIZE_T_MAX;

  err = _count_new_capacity(length, capacity, &value);
  TEST_ASSERT_EQUAL_ERROR(ARL_ERROR_OVERFLOW, err);
}

void test__grow_array_capacity_memory_failure(void) {
  arl_ptr l = setup_small_list();
  arl_error err;

  app_realloc_IgnoreAndReturn(NULL);

  err = _grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(ARL_ERROR_OUT_OF_MEMORY, err);
  TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array);
  TEST_ASSERT_EQUAL(arl_small_length, l->length);
  TEST_ASSERT_EQUAL(array_memory_mock_size / sizeof(void *), l->capacity);
}

void test__grow_array_capacity_max_failure(void) {
  arl_ptr l = setup_small_list();
  arl_error err;

  l->capacity = ARL_SIZE_T_MAX;

  err = _grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(ARL_ERROR_OVERFLOW, err);
}

void test__grow_array_capacity_success(void) {
  arl_ptr l = setup_small_list();
  size_t new_l_capacity, new_array_size;
  arl_error err;

  err = _count_new_capacity(l->length, l->capacity, &new_l_capacity);

  TEST_ASSERT_EQUAL_ERROR(ARL_SUCCESS, err);

  new_array_size = new_l_capacity * sizeof(void *);

  mock_app_realloc(l, new_array_size);

  err = _grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(ARL_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array);
  TEST_ASSERT_EQUAL(new_l_capacity, l->capacity);
}
