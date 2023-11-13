#include <stddef.h>

#include <unity.h>

#include "common/cll_error.c"

void test_errors_string_matching(void) {
  size_t error_strings_size = sizeof(CLL_ERROR_STRINGS) / sizeof(char *);

  TEST_ASSERT_EQUAL_MESSAGE(
      CLL_ERROR_LEN, error_strings_size,
      "Each error needs to have matching pair in CLL_ERROR_STRINGS.");
}
