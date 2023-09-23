/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test framework
#include "unity.h"

// App
#include "pointers_utils.h"

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
char *values_content[] = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", "",
                          "",           ""};
size_t values_len = sizeof(values_content) / sizeof(char *);
char **values = NULL, **values_empty = NULL;
/* values_ptr_cp stores value's elements pointers copy.
 * It is not meant to be modified by tests.
 * This way freeing all elements is nice and easy.
 */
void **values_ptr_cp = NULL;

char **alloc_values(size_t n) {
  size_t values_size = n * sizeof(char *);
  char **values;

  values = malloc(values_size);
  if (!values) {
    TEST_FAIL_MESSAGE("Unable to allocate memory for `values`.");
  }

  return values;
}

char **alloc_values_elements(size_t n, char *content[n], char **values) {
  size_t i, value_size;
  char *element;

  for (i = 0; i < n; i++) {
    value_size = (strlen(content[i]) + 1) * sizeof(char); // count string size

    element = malloc(value_size);
    if (!element) {
      TEST_FAIL_MESSAGE("Unable to allocate memory for `element`.");
    }

    values[i] = element;
  }

  return values;
}

void free_values(size_t n, char *values[n]) {
  for (size_t i = 0; i < n; i++)
    if (values[i])
      free(values[i]);

  free(values);
}

void setUp() {
  values = alloc_values(values_len);
  values = alloc_values_elements(values_len, values_content, values);
  for (size_t i = 0; i < values_len; i++)
    strcpy(values[i], values_content[i]);

  values_ptr_cp = malloc(values_len * sizeof(char *));
  for (size_t i = 0; i < values_len; i++)
    values_ptr_cp[i] = values[i];

  values_empty = malloc(values_len * sizeof(char *));
}

void tearDown() {
  free_values(values_len, (char **)values_ptr_cp);

  free(values);
  free(values_empty);
}

void print_values(size_t n, char *values[n]) {
  for (size_t i = 0; i < n; i++) {
    if (!values[i])
      puts("NULL, ");
    else
      printf("%s, \n", values[i]);
  }
}

void test_move_pointers_array_success(void) {
  char **dest = values_empty, **src = values;

  move_pointers_array((void **)dest, (void **)src, values_len);

  for (size_t i = 0; i < values_len; i++)
    TEST_ASSERT_NULL(src[i]);

  for (size_t i = 0; i < values_len; i++)
    TEST_ASSERT_EQUAL_STRING(values_content[i], dest[i]);
}

void test_move_pointers_array_overlapping_src_one_element_before_dest(void) {
  char *expected[] = {
      NULL, "MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", "", "",
  };

  char **dest, **src = values;
  dest = src + 1;

  // First element will be deleted

  move_pointers_array((void **)dest, (void **)src, values_len - 1);

  TEST_ASSERT_NULL(src[0]);

  for (size_t i = 0; i < values_len; i++) {
    if (!expected[i])
      TEST_ASSERT_NULL(expected[i]);
    else
      TEST_ASSERT_EQUAL_STRING(expected[i], values[i]);
  }
}

/* This scenario is hard to follow and pretty misleading, function name
 *  is not `nullify_array` afterall. That's why it's described as undefined.
 * Key to understand why all values are NULL, is in first two iterations.
 * 1. values = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", "", NULL,}
 * 2. values = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", NULL, NULL,}
 */
void test_move_pointers_array_overlapping_dest_one_element_before_src(void) {
  char *expected[] = {"", NULL, NULL, NULL, NULL, NULL, NULL};

  char **src, **dest = values;
  src = dest + 1;

  move_pointers_array((void **)dest, (void **)src, values_len - 1);

  for (size_t i = 0; i < values_len; i++) {
    if (!expected[i])
      TEST_ASSERT_NULL(expected[i]);
    else
      TEST_ASSERT_EQUAL_STRING(expected[i], values[i]);
  }
}

void test_move_pointers_array_overlapping_src_multi_element_before_dest(void) {
  char *expected[] = {
      NULL, NULL, NULL, "MumboJumbo", "Kukuryku", "EeeeeeMakarena", "",
  };

  char **dest, **src = values;
  dest = src + 3; // remember to substract offset from `n`

  move_pointers_array((void **)dest, (void **)src, values_len - 3);

  TEST_ASSERT_NULL(src[0]);

  for (size_t i = 0; i < values_len; i++) {
    if (!expected[i])
      TEST_ASSERT_NULL(expected[i]);
    else
      TEST_ASSERT_EQUAL_STRING(expected[i], values[i]);
  }
}
