#include <stddef.h>

#include <unity.h>

#include "common/l_error.c"

void test_errors_string_matching(void) {
  size_t error_strings_size = sizeof(L_ERROR_STRINGS) / sizeof(char *);

  TEST_ASSERT_EQUAL_MESSAGE(
      L_ERROR_LEN, error_strings_size,
      "Each error needs to have matching pair in L_ERROR_STRINGS.");
}
