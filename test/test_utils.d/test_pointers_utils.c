#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "pointers_utils.h"
#include "unity.h"

char *values[] = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena"};
size_t values_len = sizeof(values) / sizeof(char *);
char **values_cp;

void **memory_mock_value;

void setUp() {
  size_t i, values_size = sizeof(values);
  for (i = 0; i < values_len; i++)
    values_size += strlen(values[i]) + 1 * sizeof(char); // count string size

  void **memory_mock_local = malloc(values_size);
  if (!memory_mock_local) {
    TEST_FAIL_MESSAGE("Unable to allocate memory for `memory_mock_value`. "
                      "Mocking memory failed!");
  }

  memory_mock_value = memory_mock_local;

  memory_mock_local = malloc(values_size);
  if (!memory_mock_local) {
    TEST_FAIL_MESSAGE("Unable to allocate memory for `values_cp`. "
                      "Mocking memory failed!");
  }

  values_cp = memory_mock_local;

  for (size_t i = 0; i < values_len; i++) {
    values_cp[i] = values[i];
  }
}
void tearDown() {
  free(memory_mock_value);
  free(values_cp);
}

void test_pointers_copy_value(void) {
  char **dest = memory_mock_value, **src = values;

  move_pointers_array((void **)dest, (void **)src, values_len);

  TEST_ASSERT_EQUAL_PTR_ARRAY(values_cp, dest, values_len);

  for (size_t i = 0; i < values_len; i++)
    TEST_ASSERT_NULL(src[i]);
}
