#include <stddef.h>

#include <unity.h>

#include "arl_list.c"

void test_errors_string_matching(void) {
  size_t error_strings_size = sizeof(ARL_ERROR_STRINGS) / sizeof(char *);

  TEST_ASSERT_EQUAL_MESSAGE(
      ARL_ERROR_LEN, error_strings_size,
      "Each error needs to have matching pair in ARL_ERROR_STRINGS.");
}
