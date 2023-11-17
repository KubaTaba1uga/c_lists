/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>

#include <stddef.h>
#include <unity.h>

#include "c_lists/cll_def.h"
#include "utils/cll_overflow_utils.h"

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/
void test_cll_is_overflow_int_multi_true(void) {
  bool received;
  int a = INT_MAX, b = 2;

  received = cll_is_overflow_int_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test_cll_is_overflow_int_multi_false(void) {
  bool received;
  int a = INT_MAX, b = 1;

  received = cll_is_overflow_int_multi(a, b);

  TEST_ASSERT_FALSE(received);
}

void test_cll_is_overflow_size_t_multi_true(void) {
  bool received;
  size_t a = CLL_SIZE_T_MAX, b = 2;

  received = cll_is_overflow_size_t_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test_cll_is_overflow_size_t_multi_false(void) {
  bool received;
  size_t a = CLL_SIZE_T_MAX, b = 1;

  received = cll_is_overflow_size_t_multi(a, b);

  TEST_ASSERT_FALSE(received);
}
