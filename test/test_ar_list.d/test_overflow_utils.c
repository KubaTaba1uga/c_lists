/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <limits.h>
#include <stddef.h>
// Test framework
#include <unity.h>
// App
#include "arl_list.c"

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

void test__is_overflow_size_t_multi_true(void) {
  bool received;
  size_t a = ARL_SIZE_T_MAX, b = 2;

  received = _is_overflow_size_t_multi(a, b);

  TEST_ASSERT_TRUE(received);
}

void test__is_overflow_size_t_multi_false(void) {
  bool received;
  size_t a = ARL_SIZE_T_MAX, b = 1;

  received = _is_overflow_size_t_multi(a, b);

  TEST_ASSERT_FALSE(received);
}
