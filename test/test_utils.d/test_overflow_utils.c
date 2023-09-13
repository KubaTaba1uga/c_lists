/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>

#include <stddef.h>
#include <unity.h>

#include "l_def.h"
#include "utils/overflow_utils.h"

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/
void test_is_overflow_int_multi_true(void) {
  bool received;
  int a = INT_MAX, b = 2;

  received = is_overflow_int_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test_is_overflow_int_multi_false(void) {
  bool received;
  int a = INT_MAX, b = 1;

  received = is_overflow_int_multi(a, b);

  TEST_ASSERT_FALSE(received);
}

void test_is_overflow_size_t_multi_true(void) {
  bool received;
  size_t a = L_SIZE_T_MAX, b = 2;

  received = is_overflow_size_t_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test_is_overflow_size_t_multi_false(void) {
  bool received;
  size_t a = L_SIZE_T_MAX, b = 1;

  received = is_overflow_size_t_multi(a, b);

  TEST_ASSERT_FALSE(received);
}

void test_is_overflow_capacity_multi_true(void) {
  bool received;
  size_t a = L_CAPACITY_MAX, b = 2;

  received = is_overflow_l_capacity_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test_is_overflow_capacity_multi_false(void) {
  bool received;
  size_t a = L_CAPACITY_MAX, b = 1;

  received = is_overflow_l_capacity_multi(a, b);

  TEST_ASSERT_FALSE(received);
}
