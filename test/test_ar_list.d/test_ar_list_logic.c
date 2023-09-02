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
    l.size += 1;
  }

  return l;
}

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
 *    PUBLIC API TESTS
 ******************************************************************************/

void test_arl_init_failure(void) {
  ar_list l, *received;

  app_malloc_IgnoreAndReturn(NULL);
  get_pointer_size_IgnoreAndReturn(sizeof(void));

  received = arl_init(&l, 10);

  TEST_ASSERT_NULL(received);
}

void test_arl_init_success(void) {
  ar_list l, *received;
  size_t default_capacity = memory_mock_size / sizeof(void *);

  app_malloc_ExpectAndReturn(memory_mock_size, memory_mock);
  get_pointer_size_IgnoreAndReturn(sizeof(void *));

  received = arl_init(&l, default_capacity);

  TEST_ASSERT_NOT_NULL(received);
  TEST_ASSERT_EQUAL(l.size, 0);
  TEST_ASSERT_EQUAL(l.capacity, default_capacity);
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/
void test_arl_count_new_capacity_base(void) {
  /* Show array capacity growth ratio by example. */
  /* Purpose of this function is mainly documentational. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t size = 0;
  size_t capacity = 1000;
  size_t i;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    capacity = size = arl_count_new_capacity_base(size, capacity);

    TEST_ASSERT_EQUAL(capacity, expected_values[i]);
  }
}

void test_arl_is_i_invalid_true(void) {
  ar_list ll[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ll) / sizeof(ar_list); i++) {
    ar_list l = ll[i];

    print_array_values(l);

    size_t j, i_to_check[] = {l.size, ULONG_MAX};
    bool is_invalid;

    for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
      is_invalid = arl_is_i_invalid(&l, i_to_check[j]);

      TEST_ASSERT_TRUE(is_invalid);
    }
  }
}

/* void test_arl_is_i_invalid_false(void) { */
/*   ar_list l = setup_small_list(); */

/*     size_t j, i_to_check[] = {l.size, ULONG_MAX}; */
/*     bool is_invalid; */

/*     for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*       is_invalid = arl_is_i_invalid(&l, i_to_check[j]); */

/*       TEST_ASSERT_TRUE(is_invalid); */
/*     } */
/*   } */
/* } */

/* void test_arl_alloc_array */
