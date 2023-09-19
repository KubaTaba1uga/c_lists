#include <stdlib.h>
#include <string.h>

#include "pointers_utils.h"
#include "unity.h"

char value[] = "SuperDuper";
char *values[] = {"MumboJumbo", "Kukuryku", "EeeeeeMakarena"};
void **memory_mock_value;

void setUp() {
  void **memory_mock_local = malloc(sizeof(void *) * 4096);
  if (!memory_mock_local) {
    TEST_FAIL_MESSAGE("Unable to allocate memory for `memory_mock_value`. "
                      "Mocking memory failed!");
  }

  memory_mock_value = memory_mock_local;
}
void tearDown() { free(memory_mock_value); }

void test_pointers_copy_value(void) {
  char **dest = memory_mock_value, **src = values;

  pointers_copy(3, (void **)dest, 3, (void **)src);

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */

  /* pointers_move((void **)dest, (void **)src, 2); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */

  puts("");
  printf("source: %s\n", src[0]);
  printf("dest: %s\n", dest[0]);
  printf("source: %s\n", src[1]);
  printf("dest: %s\n", dest[1]);
  printf("source: %s\n", src[2]);
  printf("dest: %s\n", dest[2]);

  puts("");
  /* pointers_move((void **)dest, (void **)src, 3); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */
  /* TEST_ASSERT_EQUAL_PTR(src[2], dest[2]); */

  /* pointers_move((void **)dest, (void **)src, 4); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */
  /* TEST_ASSERT_EQUAL_PTR(src[2], dest[2]); */
  /* TEST_ASSERT_EQUAL_PTR(src[3], dest[3]); */
}

void test_pointers_move_value(void) {
  char *dest = memory_mock_value, *src = value;

  /* pointers_move((void **)&dest, (void **)src, 1); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */

  /* pointers_move((void **)dest, (void **)src, 2); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */

  /* puts(""); */
  /* printf("source: %s\n", src); */
  /* printf("dest: %s\n", dest); */
  /* puts(""); */
  /* pointers_move((void **)dest, (void **)src, 3); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */
  /* TEST_ASSERT_EQUAL_PTR(src[2], dest[2]); */

  /* pointers_move((void **)dest, (void **)src, 4); */

  /* TEST_ASSERT_EQUAL_PTR(src[0], dest[0]); */
  /* TEST_ASSERT_EQUAL_PTR(src[1], dest[1]); */
  /* TEST_ASSERT_EQUAL_PTR(src[2], dest[2]); */
  /* TEST_ASSERT_EQUAL_PTR(src[3], dest[3]); */
}
