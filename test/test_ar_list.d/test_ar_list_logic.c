/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <stddef.h>
#include <stdlib.h>

#include <unity.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "ar_list.c"

#include "mock_std_lib_utils.h"
#include "std_lib_utils.h"

/*******************************************************************************
 *    TESTS DECLARATIONS
 ******************************************************************************/
int *l_values;
size_t l_values_size;

size_t memory_mock_size = 0;
void *memory_mock = NULL;

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
int arl_small_values[] = {0, 1, 2, 3, 4, 5};

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  memory_mock_size = sizeof(void) * 10;

  memory_mock = malloc(memory_mock_size);
  if (!memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory. Mocking malloc failed!");
}

void tearDown(void) {
  if (memory_mock != NULL) {
    free(memory_mock);
    memory_mock = NULL;
  }
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
  size_t default_capacity = memory_mock_size / sizeof(void);

  app_malloc_ExpectAndReturn(memory_mock_size, memory_mock);
  get_pointer_size_IgnoreAndReturn(sizeof(void));

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

/* void test_arl_alloc_array */
