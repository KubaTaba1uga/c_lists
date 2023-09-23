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
char **values = NULL, **values_cp = NULL;
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

// TO-DO refacotr setUp and tearDown, setUp make mallocs tearDown make frees
//            configure their own environment using memory mocks.
void setUp() {
  char **mem, ***arrays_to_fill[] = {&values, &values_cp};

  for (size_t i = 0; i < sizeof(arrays_to_fill) / sizeof(char *); i++) {
    mem = alloc_values(values_len);
    mem = alloc_values_elements(values_len, values_content, mem);
    for (size_t i = 0; i < values_len; i++)
      strcpy(mem[i], values_content[i]);

    *(arrays_to_fill[i]) = mem;
  }

  values_ptr_cp = calloc(values_len, values_len * sizeof(char *));
}

void tearDown() {
  free_values(values_len, values);
  free_values(values_len, values_cp);
  free_values(values_len, values_ptr_cp);
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
  char **dest = (char **)values_ptr_cp, **src = values;

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

  move_pointers_array((void **)dest, (void **)src, values_len - 1);

  TEST_ASSERT_NULL(src[0]);

  for (size_t i = 1; i < values_len; i++)
    TEST_ASSERT_EQUAL_STRING(expected[i], src[i]);
}

/* /\* This scenario is hard to follow and pretty misleading, function name */
/*  *  is not `nullify_array` afterall. That's why it's described as undefined.
 */
/*  * Key to understand why all values are NULL, is in first two iterations. */
/*  * 1. values = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", "", NULL,} */
/*  * 2. values = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", NULL, NULL,}
 */
/*  *\/ */
/* void test_move_pointers_array_overlapping_dest_one_element_before_src(void) {
 */
/*   char *expected[] = {"", NULL, NULL, NULL, NULL, NULL, NULL}; */

/*   char **src, **dest = values; */
/*   src = dest + 1; */

/*   move_pointers_array((void **)dest, (void **)src, values_len - 1); */

/*   TEST_ASSERT_EQUAL_STRING(expected[0], values[0]); */

/*   for (size_t i = 1; i < values_len; i++) */
/*     TEST_ASSERT_NULL(values[i]); */
/* } */

/* void test_move_pointers_array_overlapping_src_multi_element_before_dest(void)
 * { */
/*   char *expected[] = { */
/*       NULL, NULL, NULL, "MumboJumbo", "Kukuryku", "EeeeeeMakarena", "", */
/*   }; */

/*   char **dest, **src = values; */
/*   dest = src + 3; */

/*   move_pointers_array((void **)dest, (void **)src, values_len - 3); */

/*   print_values(values_len, values); */

/*   TEST_ASSERT_NULL(src[0]); */

/*   for (size_t i = 0; i < values_len; i++) { */
/*     if (!expected[i]) */
/*       TEST_ASSERT_NULL(expected[i]); */
/*     else */
/*       TEST_ASSERT_EQUAL_STRING(expected[i], values[i]); */
/*   } */
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
