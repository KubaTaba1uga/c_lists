/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include <unity.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "ar_list.c"
#include "l_def.h"
#include "l_error.h"

#include "mock_std_lib_utils.h"
#include "test_utils.h"

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
size_t memory_mock_size = 0;
void *memory_mock = NULL;
int arl_small_values[] = {0, 1, 2, 3, 4, 5};
size_t arl_small_size = sizeof(arl_small_values) / sizeof(int);

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  memory_mock_size = sizeof(void *) * 10;

  memory_mock = malloc(memory_mock_size);
  if (!memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory. Mocking malloc failed!");

  get_pointer_size_IgnoreAndReturn(sizeof(void *));
}

void tearDown(void) {
  if (!memory_mock)
    return;

  free(memory_mock);
  memory_mock = NULL;
  memory_mock_size = 0;
}

/*******************************************************************************
 *    TESTS UTILS
 ******************************************************************************/

ar_list setup_empty_list() {
  ar_list l;
  size_t default_capacity = memory_mock_size / sizeof(void *);

  app_malloc_ExpectAndReturn(memory_mock_size, memory_mock);

  arl_init(&l, default_capacity);

  return l;
}
ar_list setup_small_list() {
  ar_list l = setup_empty_list();
  size_t i;

  for (i = 0; i < arl_small_size; i++) {
    l.array[i] = &arl_small_values[i];
    l.length += 1;
  }

  return l;
}

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

/* void test_arl_init_memory_failure(void) { */
/*   ar_list l; */
/*   int received; */

/*   app_malloc_IgnoreAndReturn(NULL); */

/*   received = arl_init(&l, 10); */

/*   TEST_ASSERT_EQUAL(L_ERROR_MEMORY_SHORTAGE, received); */
/* } */

/* void test_arl_init_overflow_failure(void) { */
/*   ar_list l; */
/*   int received; */

/*   received = arl_init(&l, ULONG_MAX); */

/*   TEST_ASSERT_EQUAL(L_ERROR_OVERFLOW, received); */
/* } */

/* void test_arl_init_success(void) { */
/*   ar_list l; */
/*   int received; */
/*   size_t default_capacity = memory_mock_size / sizeof(void *); */

/*   app_malloc_ExpectAndReturn(memory_mock_size, memory_mock); */

/*   received = arl_init(&l, default_capacity); */

/*   TEST_ASSERT_EQUAL(received, 0); */
/*   TEST_ASSERT_EQUAL(l.size, 0); */
/*   TEST_ASSERT_EQUAL(l.capacity, default_capacity); */
/* } */

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

void test_arl_count_new_capacity_overflow(void) {
  /* Show array growth stop. */
  /* Upper bound of list is well defined. */
  size_t length, capacity;

  capacity = length = L_CAPACITY_MAX;

  capacity = arl_count_new_capacity(length, capacity);

  TEST_ASSERT_EQUAL(L_CAPACITY_MAX, capacity);
}

/* void test_arl_is_i_too_big_true(void) { */
/*   ar_list ll[] = {setup_empty_list(), setup_small_list()}; */
/*   l_error_t err; */

/*   for (size_t i = 0; i < sizeof(ll) / sizeof(ar_list); i++) { */
/*     ar_list l = ll[i]; */

/*     size_t j, i_to_check[] = {l.size, ULONG_MAX}; */
/*     bool is_invalid; */

/*     for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*       err = arl_is_i_too_big(&l, i_to_check[j], &is_invalid); */

/*       TEST_ASSERT_EQUAL(L_SUCCESS, err); */
/*       TEST_ASSERT_TRUE(is_invalid); */
/*     } */
/*   } */
/* } */

/* void test_arl_is_i_too_big_false(void) { */
/*   ar_list l = setup_small_list(); */
/*   l_error_t err; */

/*   size_t j, i_to_check[] = {l.size - 1, 0}; */
/*   bool is_invalid; */

/*   for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*     err = arl_is_i_too_big(&l, i_to_check[j], &is_invalid); */

/*     TEST_ASSERT_EQUAL(L_SUCCESS, err); */
/*     TEST_ASSERT_FALSE(is_invalid); */
/*   } */
/* } */

/* void test_arl_grow_array_capacity_memory_failure(void) { */
/*   ar_list l_cp, l = setup_small_list(); */
/*   l_error_t err; */

/*   l_cp = l; */

/*   app_realloc_IgnoreAndReturn(NULL); */

/*   err = arl_grow_array_capacity(&l); */

/*   TEST_ASSERT_EQUAL(L_ERROR_MEMORY_SHORTAGE, err); */
/*   TEST_ASSERT_EQUAL_PTR(l_cp.array, l.array); */
/*   TEST_ASSERT_EQUAL(l_cp.size, l.size); */
/*   TEST_ASSERT_EQUAL(l_cp.capacity, l.capacity); */
/* } */

/* void test_arl_grow_array_capacity_overflow(void) { */
/*   ar_list l = setup_small_list(); */
/*   l_error_t err; */

/*   l.capacity = ULONG_MAX; */
/*   l.size = ULONG_MAX; */

/*   err = arl_grow_array_capacity(&l); */

/*   TEST_ASSERT_EQUAL(L_ERROR_OVERFLOW, err); */
/* } */

/* void test_arl_grow_array_capacity_success(void) { */
/*   l_error_t err; */
/*   void *new_array; */
/*   ar_list l = setup_small_list(); */
/*   size_t new_l_capacity, new_array_size; */

/*   err = arl_count_new_capacity(l.size, l.capacity, &new_l_capacity); */
/*   TEST_ASSERT_EQUAL(L_SUCCESS, err); */

/*   new_array_size = new_l_capacity * get_pointer_size(); */

/*   new_array = realloc(l.array, new_array_size); */
/*   if (!new_array) */
/*     TEST_FAIL_MESSAGE("Unable to reallocate memory!"); */

/*   memory_mock = new_array; */

/*   app_realloc_ExpectAndReturn(l.array, new_array_size, new_array); */

/*   err = arl_grow_array_capacity(&l); */

/*   TEST_ASSERT_EQUAL(L_SUCCESS, err); */
/*   TEST_ASSERT_EQUAL_PTR(new_array, l.array); */
/*   TEST_ASSERT_EQUAL(new_l_capacity, l.capacity); */
/* } */

/* void test_arl_move_indexes_by_positive_number_new_size_overflow(void) { */
/*   /\* int exit_code; *\/ */
/*   /\* ar_list l = setup_small_list(); *\/ */

/*   /\* l.size =  *\/ */
/* } */
/* /\* void test_arl_alloc_array *\/ */
