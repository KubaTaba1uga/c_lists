#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "pointers_utils.h"
#include "unity.h"

char **values,
    **values_ptr_cp; // = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena",
                     // "", "", "", ""};
size_t values_len;

void **memory_mock_values;

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
    free(values[i]);

  free(values);
}

void setUp() {
  char *values_content[] = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", "",
                            "",           ""};
  size_t i;
  values_len = sizeof(values_content) / sizeof(char *);

  values = alloc_values(values_len);
  values = alloc_values_elements(values_len, values_content, values);
  for (i = 0; i < values_len; i++)
    strcpy(values[i], values_content[i]);

  // Copy values pointers to values_cp
  values_ptr_cp = alloc_values(values_len);

  for (size_t i = 0; i < values_len; i++) {
    values_ptr_cp[i] = values[i];
  }

  // Allocate mock
  memory_mock_values = (void **)alloc_values(values_len);
}
void tearDown() {
  free_values(values_len, values_ptr_cp);
  free(memory_mock_values);
  free_values(values_len, values);
}

void test_pointers_copy_value(void) {
  char **dest = memory_mock_values, **src = values;

  move_pointers_array((void **)dest, (void **)src, values_len);

  TEST_ASSERT_EQUAL_PTR_ARRAY(values_ptr_cp, dest, values_len);

  for (size_t i = 0; i < values_len; i++)
    TEST_ASSERT_NULL(src[i]);
}

/* void test_pointers_copy_2(void) { */
/*   char **dest = memory_mock_values, **src = values; */

/*   move_pointers_array((void **)dest, (void **)src, values_len); */

/*   TEST_ASSERT_EQUAL_PTR_ARRAY(values_ptr_cp, dest, values_len); */

/*   for (size_t i = 0; i < values_len; i++) */
/*     TEST_ASSERT_NULL(src[i]); */
/* } */

/* void test_move_pointers_array_overlapping_src_one_element_before_dest(void)
 * {} */
/* #include <stddef.h> */
/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */

/* #include "pointers_utils.h" */
/* #include "unity.h" */

/* char *values[] = {"MumboJumbo", "Kukuryku", */
/*                   "EeeeeeMakarena"}; //, "", "", "", ""}; */
/* size_t values_len = sizeof(values) / sizeof(char *); */
/* char **values_cp; */

/* void **memory_mock_value; */

/* void setUp() { */
/*   size_t i, values_size = sizeof(values); */

/*   for (i = 0; i < values_len; i++) */
/*     values_size += (strlen(values[i]) + 1) * sizeof(char); // count string
 * size */

/*   void **memory_mock_local = malloc(values_size); */
/*   if (!memory_mock_local) { */
/*     TEST_FAIL_MESSAGE("Unable to allocate memory for `memory_mock_value`. "
 */
/*                       "Mocking memory failed!"); */
/*   } */

/*   memory_mock_value = memory_mock_local; */

/*   memory_mock_local = malloc(values_size); */
/*   if (!memory_mock_local) { */
/*     TEST_FAIL_MESSAGE("Unable to allocate memory for `values_cp`. " */
/*                       "Mocking memory failed!"); */
/*   } */

/*   values_cp = (char **)memory_mock_local; */

/*   for (size_t i = 0; i < values_len; i++) { */
/*     values_cp[i] = values[i]; */
/*   } */
/* } */
/* void tearDown() { */
/*   free(memory_mock_value); */
/*   free(values_cp); */
/* } */

/* void test_move_pointers_array_no_overlapping(void) { */
/*   char **dest = memory_mock_value, **src = values; */

/*   move_pointers_array((void **)dest, (void **)src, 3); */

/*   /\* TEST_ASSERT_EQUAL_PTR_ARRAY(values_cp, dest, values_len); *\/ */

/*   /\* for (size_t i = 0; i < values_len; i++) *\/ */
/*   /\*   TEST_ASSERT_NULL(src[i]); *\/ */
/* } */
