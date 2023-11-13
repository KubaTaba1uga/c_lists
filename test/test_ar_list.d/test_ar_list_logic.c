/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test framework
#include "mock_std_lib_interface.h"
#include <unity.h>

// App
/* Including source file instead of header file allows
 *  for static functions testing.
 *  Do not forget adding `-zmuldefs` to gcc.*/
#define local_malloc malloc
#define local_free free
#define local_realloc realloc
#undef malloc
#undef realloc
#undef free
#include "ar_list.c"
#undef malloc
#undef realloc
#undef free
#define malloc local_malloc
#define realloc local_realloc
#define free local_free

#include "l_def.h"
#include "l_error.h"

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
size_t list_memory_mock_size = 0, array_memory_mock_size = 0;
void *list_memory_mock = NULL, *array_memory_mock = NULL;
int arl_small_values[] = {0, 1, 2, 3, 4, 5};
size_t arl_small_length = sizeof(arl_small_values) / sizeof(int);
size_t FREE_COUNTER = 0;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void) {
  FREE_COUNTER = 0;

  array_memory_mock_size = L_PTR_SIZE * 10;

  array_memory_mock = malloc(array_memory_mock_size);
  if (!array_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory for `array_memory_mock`. "
                      "Mocking malloc failed!");

  list_memory_mock_size = sizeof(struct ar_list);

  list_memory_mock = malloc(list_memory_mock_size);
  if (!list_memory_mock)
    TEST_FAIL_MESSAGE("Unable to allocate memory for `list_memory_mock`. "
                      "Mocking malloc failed!");
}

void tearDown(void) {

  if (array_memory_mock)
    free(array_memory_mock);

  if (list_memory_mock)
    free(list_memory_mock);

  array_memory_mock = NULL;
  array_memory_mock = 0;

  list_memory_mock = NULL;
  list_memory_mock = 0;
}

/*******************************************************************************
 *    TESTS UTILS
 ******************************************************************************/
arl_ptr setup_empty_list() {
  arl_ptr l;
  size_t default_capacity = array_memory_mock_size / L_PTR_SIZE;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  arl_create(&l, default_capacity);

  return l;
}

arl_ptr setup_small_list() {
  arl_ptr l = setup_empty_list();
  size_t i;

  for (i = 0; i < arl_small_length; i++) {
    l->array[i] = &arl_small_values[i];
    l->length += 1;
  }

  return l;
}

void mock_app_realloc(arl_ptr l, size_t new_array_size) {
  // Do not use realloc, it would invalidate l->array
  // before actual function execution.
  void *new_array;

  new_array = malloc(new_array_size);
  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to allocate memory for realloc mock!");

  new_array = memcpy(new_array, l->array, l->capacity * L_PTR_SIZE);

  if (!new_array)
    TEST_FAIL_MESSAGE("Unable to copy memory for realloc mock!");

  free(array_memory_mock);

  array_memory_mock = new_array;

  app_realloc_ExpectAndReturn(l->array, new_array_size, new_array);
}

void dummy_free(void *value) { FREE_COUNTER = FREE_COUNTER + 1; }

void print_src_dest(void *p) {
  if (!p)
    printf("NULL, ");
  else
    printf("%i, ", *(int *)p);
}

void print_list(arl_ptr l, size_t n) {
  printf("l->array = [");
  for (size_t j = 0; j < n; j++) {
    print_src_dest(l->array[j]);
  }
  puts("]");
}

void parametrize_test_arl_get_i_too_big_failure(arl_ptr l);
void parametrize_test_arl_set_i_too_big_failure(arl_ptr l);
void parametrize_test_arl_insert_success(arl_ptr l, size_t i, int value);

void TEST_ASSERT_EQUAL_ERROR(l_error_t expected, l_error_t received) {
  TEST_ASSERT_EQUAL_STRING(l_strerror(expected), l_strerror(received));
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/
void test_arl_count_new_capacity(void) {
  /* Show array capacity growth ratio by example. */
  /* Purpose of this function is mainly documentational. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t length = 0;
  size_t capacity = 1000;
  size_t i;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    capacity = length = arl_count_new_capacity(length, capacity);

    TEST_ASSERT_EQUAL(expected_values[i], capacity);
  }
}

void test_arl_count_new_capacity_overflow_failure(void) {
  /* Show array growth stop. */
  /* Upper bound of list is well defined. */
  size_t length, capacity;

  capacity = length = ARL_CAPACITY_MAX;

  capacity = arl_count_new_capacity(length, capacity);

  TEST_ASSERT_EQUAL(ARL_CAPACITY_MAX, capacity);
}

void test_arl_is_i_too_big_true(void) {
  arl_ptr l = setup_small_list();
  size_t j, i_to_check[] = {l->length, l->length + 1, l->capacity};
  bool is_i_too_big;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_i_too_big = arl_is_i_too_big(l, i_to_check[j]);

    TEST_ASSERT_TRUE(is_i_too_big);
  }
}

void test_arl_is_i_too_big_false(void) {
  arl_ptr l = setup_small_list();
  size_t j, i_to_check[] = {l->length - 1, 0};
  bool is_i_too_big;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_i_too_big = arl_is_i_too_big(l, i_to_check[j]);

    TEST_ASSERT_FALSE(is_i_too_big);
  }
}

void test_arl_grow_array_capacity_memory_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  app_realloc_IgnoreAndReturn(NULL);

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_OUT_OF_MEMORY, err);
  TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array);
  TEST_ASSERT_EQUAL(arl_small_length, l->length);
  TEST_ASSERT_EQUAL(array_memory_mock_size / sizeof(void *), l->capacity);
}

void test_arl_grow_array_capacity_max_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  l->capacity = ARL_CAPACITY_MAX;

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_REACHED_CAPACITY_MAX, err);
}

void test_arl_grow_array_capacity_success(void) {
  arl_ptr l = setup_small_list();
  size_t new_l_capacity, new_array_size;
  l_error_t err;

  new_l_capacity = arl_count_new_capacity(l->length, l->capacity);

  new_array_size = new_l_capacity * L_PTR_SIZE;

  mock_app_realloc(l, new_array_size);

  err = arl_grow_array_capacity(l);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(array_memory_mock, l->array);
  TEST_ASSERT_EQUAL(new_l_capacity, l->capacity);
}

void test_arl_move_elements_right_new_length_overflow_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_right(l, 0, L_SIZE_T_MAX);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_OVERFLOW, err);
}

void test_arl_move_elements_right_new_length_invalid(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_right(l, 0, l->capacity + 1);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_INVALID_ARGS, err);
}

void test_arl_move_elements_right_elements_to_move_amount_underflow_failure(
    void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_right(l, l->length + 1, 0);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_right_no_elements_to_move(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_right(l, l->length, 0);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
}

/* Confirms:
 *    INPUT  l.array {0, 1, 2, 3, 4, 5, , , , }, start_i 1, move_by 2
 *    OUTPUT l.array {0, NULL, NULL, 1, 2, 3, 4, 5, , }
 */
void test_arl_move_elements_right_success(void) {
  int null_indexes[] = {1, 2, 3};
  size_t null_i_length = sizeof(null_indexes) / sizeof(int);

  arl_ptr l = setup_small_list();
  size_t i;
  int *value;
  l_error_t err;

  err = arl_move_elements_right(l, 1, 3);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(9, l->length);

  // Values before start_i should'n be changed
  for (i = 0; i < null_indexes[0]; i++) {
    err = arl_get(l, i, (void **)&value);
    TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);

    TEST_ASSERT_EQUAL(arl_small_values[i], *value);
  }

  // NULLs should be inserted in old values place
  for (i = 0; i < null_i_length; i++) {
    err = arl_get(l, null_indexes[i], (void **)&value);
    TEST_ASSERT_NULL(value);
  }

  // Old values are moved by move_by
  int last_null_index = null_indexes[null_i_length - 1];

  for (i = last_null_index + 1; i < l->length; i++) {
    err = arl_get(l, i, (void **)&value);
    TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);

    TEST_ASSERT_EQUAL(arl_small_values[i - last_null_index], *value);
  }
}

void test_arl_move_elements_left_new_length_underflow_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_left(l, l->length + 1, l->length + 1);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_left_elements_to_move_underflow_failure(void) {
  arl_ptr l = setup_small_list();
  l_error_t err;

  err = arl_move_elements_left(l, l->length + 1, 0);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_UNDERFLOW, err);
}

void test_arl_move_elements_left_delete_index_0(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[1], l->array[2], l->array[3],
                      l->array[4], l->array[5], NULL};
  l_error_t err;

  err = arl_move_elements_left(l, 1, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
}

void test_arl_move_elements_left_delete_index_1(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[2], l->array[3],
                      l->array[4], l->array[5], NULL};
  l_error_t err;

  err = arl_move_elements_left(l, 2, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
}

void test_arl_move_elements_left_delete_index_2(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[3],
                      l->array[4], l->array[5], NULL};
  l_error_t err;

  err = arl_move_elements_left(l, 3, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
}

void test_arl_move_elements_left_delete_index_3(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2],
                      l->array[4], l->array[5], NULL};
  l_error_t err;

  err = arl_move_elements_left(l, 4, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
}

void test_arl_move_elements_left_delete_index_4(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2],
                      l->array[3], l->array[5], NULL};
  l_error_t err;

  err = arl_move_elements_left(l, 5, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
}

void test_arl_move_elements_left_delete_index_5(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2],
                      l->array[3], l->array[4], NULL};
  l_error_t err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 1);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(5, l->length);
}

void test_arl_move_elements_left_delete_last_indexes_0(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2],
                      l->array[3], NULL,        NULL};
  l_error_t err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 2);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(4, l->length);
}

void test_arl_move_elements_left_delete_last_indexes_1(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[1], l->array[2], NULL, NULL, NULL};
  l_error_t err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 3);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(3, l->length);
}

void test_arl_move_elements_left_delete_all(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {NULL, NULL, NULL, NULL, NULL, NULL};
  l_error_t err;

  // Reaching over the array's boundary, is handled.
  err = arl_move_elements_left(l, 6, 999);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(0, l->length);
}

void test_arl_move_elements_left_move_by_bigger_than_start_i(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[4], l->array[5]};
  l_error_t err;

  err = arl_move_elements_left(l, 4, 5);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, l->length);
}

/* Confirms:
 *    INPUT  l.array {0, 1, 2, 3, 4, 5, , , , }, start_i 3, move_by 2
 *    OUTPUT l.array {0, 3, 4, 5, , , , , , }
 */
void test_arl_move_elements_left_success(void) {
  arl_ptr l = setup_small_list();
  void *expected[] = {l->array[0], l->array[3], l->array[4], l->array[5]};
  l_error_t err;

  err = arl_move_elements_left(l, 3, 2);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, l->array, l->length);
}

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

void test_arl_create_memory_failure_array(void) {
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, NULL);

  err = arl_create(&l, 10);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_OUT_OF_MEMORY, err);
}

void test_arl_create_memory_failure_list(void) {
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, NULL);
  array_memory_mock = NULL;

  err = arl_create(&l, 10);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_OUT_OF_MEMORY, err);
}

void test_arl_create_overflow_failure(void) {
  arl_ptr l;
  l_error_t err;

  err = arl_create(&l, ARL_CAPACITY_MAX + 1);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_OVERFLOW, err);
}

void test_arl_create_success(void) {
  size_t default_capacity = array_memory_mock_size / sizeof(void *);
  arl_ptr l;
  l_error_t err;

  app_malloc_ExpectAndReturn(array_memory_mock_size, array_memory_mock);
  app_malloc_ExpectAndReturn(list_memory_mock_size, list_memory_mock);

  err = arl_create(&l, default_capacity);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(l->length, 0);
  TEST_ASSERT_EQUAL(l->capacity, default_capacity);
}

void test_arl_get_i_too_big_failure(void) {
  arl_ptr ls_to_param[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ls_to_param) / sizeof(arl_ptr); i++)
    parametrize_test_arl_get_i_too_big_failure(ls_to_param[i]);
}

void parametrize_test_arl_get_i_too_big_failure(arl_ptr l) {
  size_t i, indexes_to_check[] = {l->length, l->length + 1, l->length + 2};
  void *p;
  l_error_t err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_get(l, indexes_to_check[i], &p);
    TEST_ASSERT_EQUAL_ERROR(L_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_get_success(void) {
  arl_ptr l = setup_small_list();
  int *value, i;
  l_error_t err;

  for (i = 0; i < l->length; i++) {
    err = arl_get(l, i, (void **)&value);

    TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
    TEST_ASSERT_EQUAL(arl_small_values[i], *value);
  }
}

void test_arl_set_i_too_big_failure(void) {
  arl_ptr ls_to_param[] = {setup_empty_list(), setup_small_list()};

  for (size_t i = 0; i < sizeof(ls_to_param) / sizeof(arl_ptr); i++)
    parametrize_test_arl_set_i_too_big_failure(ls_to_param[i]);
}

void parametrize_test_arl_set_i_too_big_failure(arl_ptr l) {
  size_t i, indexes_to_check[] = {l->length, l->length + 1, l->length + 2};
  char value[] = "ABC";
  l_error_t err;

  for (i = 0; i < sizeof(indexes_to_check) / sizeof(size_t); i++) {
    err = arl_set(l, indexes_to_check[i], value);
    TEST_ASSERT_EQUAL_ERROR(L_ERROR_INDEX_TOO_BIG, err);
  }
}

void test_arl_set_success(void) {
  arl_ptr l = setup_small_list();
  int i, value = 13;
  l_error_t err;

  for (i = 0; i < arl_small_length; i++) {
    err = arl_set(l, i, (void *)&value);

    TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
    TEST_ASSERT_EQUAL_PTR(&value, l->array[i]);
    TEST_ASSERT_EQUAL(value, *(int *)(l->array[i]));
  }
}

void test_arl_append_grow_array_capacity(void) {

  arl_ptr l = setup_small_list();
  int value = 13;
  size_t i, new_capacity, new_array_size;
  l_error_t err;

  void *expected[] = {&arl_small_values[0],
                      &arl_small_values[1],
                      &arl_small_values[2],
                      &arl_small_values[3],
                      &arl_small_values[4],
                      &arl_small_values[5],
                      &value,
                      &value,
                      &value,
                      &value,
                      &value};
  size_t expected_length = sizeof(expected) / sizeof(void *);

  // Fill array, so next append means growing
  for (i = l->length; i < l->capacity; i++) {
    err = arl_append(l, expected[i]);
    TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  }

  new_capacity = arl_count_new_capacity(l->length, l->capacity);
  new_array_size = new_capacity * L_PTR_SIZE;

  mock_app_realloc(l, new_array_size);

  // Growing
  err = arl_append(l, &value);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(new_capacity, l->capacity);
  TEST_ASSERT_EQUAL(expected_length, l->length);
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, l->array, expected_length);
}

void test_arl_append_success(void) {
  arl_ptr l = setup_small_list();
  size_t old_len = l->length;
  int value = 13;
  l_error_t err;

  err = arl_append(l, &value);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(old_len + 1, l->length);
  TEST_ASSERT_EQUAL_PTR(&value, l->array[old_len]);
  TEST_ASSERT_EQUAL(value, *(int *)(l->array[old_len]));
}

void test_arl_insert_success(void) {
  int value = 13;
  size_t i, is_to_check[] = {0, 1, 2, 3, 4, 5};
  arl_ptr l;

  for (i = 0; i < sizeof(is_to_check) / sizeof(size_t); i++) {
    l = setup_small_list();

    parametrize_test_arl_insert_success(l, is_to_check[i], value);

    resetTest();
  }
}

void parametrize_test_arl_insert_success(arl_ptr l, size_t i, int value) {

  size_t k, old_len = l->length;
  l_error_t err;

  err = arl_insert(l, i, &value);

  // Check inserted value
  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(old_len + 1, l->length);
  TEST_ASSERT_EQUAL_PTR(&value, l->array[i]);
  TEST_ASSERT_EQUAL(value, *(int *)(l->array[i]));

  // Check not moved values
  for (k = 0; k < i; k++) {
    TEST_ASSERT_NOT_NULL(l->array[k]);
    TEST_ASSERT_EQUAL(arl_small_values[k], *(int *)l->array[k]);
  }
  // Check moved values
  for (k = i + 1; k < l->length; k++) {
    TEST_ASSERT_NOT_NULL(l->array[k]);
    TEST_ASSERT_EQUAL(arl_small_values[k - 1], *(int *)l->array[k]);
  }
}
void test_arl_pop_i_overflow(void) {

  void *expected, *received;
  size_t len_cp;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[0], l->array[1], l->array[2],
                              l->array[3], l->array[4], NULL};

  len_cp = l->length;
  i = 999;
  _arl_get(l, len_cp - 1, &expected);

  err = arl_pop(l, i, &received);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(expected, received);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
}

void test_arl_pop_empty_list(void) {

  void *received;
  l_error_t err;
  arl_ptr l = setup_empty_list();

  err = arl_pop(l, 0, &received);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_POP_EMPTY_LIST, err);
}

void test_arl_pop_first_success(void) {

  void *expected, *received;
  size_t len_cp;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[1], l->array[2], l->array[3],
                              l->array[4], l->array[5], NULL};

  len_cp = l->length;
  i = 0;
  _arl_get(l, i, &expected);

  err = arl_pop(l, i, &received);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(expected, received);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
}

void test_arl_pop_middle_success(void) {

  void *expected, *received;
  size_t len_cp;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[0], l->array[1], l->array[2],
                              l->array[4], l->array[5], NULL};

  len_cp = l->length;
  i = 3;
  _arl_get(l, i, &expected);

  err = arl_pop(l, i, &received);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(expected, received);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
}

void test_arl_pop_last_success(void) {

  void *expected, *received;
  size_t len_cp;
  l_error_t err;
  size_t i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[0], l->array[1], l->array[2],
                              l->array[3], l->array[4], NULL};

  len_cp = l->length;
  i = l->length - 1;
  _arl_get(l, i, &expected);

  err = arl_pop(l, i, &received);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR(expected, received);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
}

void test_arl_remove_success_no_callback(void) {
  size_t len_cp;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[0], l->array[2], l->array[3],
                              l->array[4], l->array[5], NULL};

  len_cp = l->length;
  i = 1;

  err = arl_remove(l, i, NULL);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(0, FREE_COUNTER);
}

void test_arl_remove_success_callback(void) {
  size_t len_cp;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();
  void *array_after_move[] = {l->array[0], l->array[2], l->array[3],
                              l->array[4], l->array[5], NULL};

  len_cp = l->length;
  i = 1;

  err = arl_remove(l, i, dummy_free);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(len_cp - 1, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array, arl_small_length);
  TEST_ASSERT_EQUAL(1, FREE_COUNTER);
}

void test_arl_insert_multi_success(void) {
  size_t len_cp, len_to_insert;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();

  void *to_insert[] = {l->array[0], l->array[2]};
  len_to_insert = sizeof(to_insert) / sizeof(void *);
  void *array_after_move[] = {l->array[0], l->array[1], l->array[0],
                              l->array[2], l->array[2], l->array[3],
                              l->array[4], l->array[5]};

  len_cp = l->length;
  i = 2;

  err = arl_insert_multi(l, i, len_to_insert, to_insert);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(len_cp + len_to_insert, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(array_after_move, l->array,
                              sizeof(array_after_move) / sizeof(void *));
}

void test_arl_slice_start_i_too_big(void) {
  size_t slice_len;
  l_error_t err;
  int i;
  void *p;
  arl_ptr l = setup_small_list();

  slice_len = 0;

  i = l->length;

  err = arl_slice(l, i, slice_len, &p);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_INDEX_TOO_BIG, err);
}

void test_arl_slice_too_much_elements_to_move(void) {
  size_t slice_len;
  l_error_t err;
  int i;
  void *p;
  arl_ptr l = setup_small_list();

  slice_len = l->length;

  i = 0;

  err = arl_slice(l, i, slice_len, &p);

  TEST_ASSERT_EQUAL_ERROR(L_ERROR_INVALID_ARGS, err);
}

void test_arl_slice_success(void) {
  size_t slice_len;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();

  slice_len = 3;
  void *slice[slice_len];

  void *expected[] = {l->array[1], l->array[2], l->array[3]};

  i = 1;

  err = arl_slice(l, i, slice_len, slice);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected, slice,
                              sizeof(expected) / sizeof(void *));
}

void test_arl_pop_multi_success_0(void) {
  size_t slice_len;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();

  slice_len = 3;
  void *slice[slice_len];

  void *expected_slice[] = {l->array[1], l->array[2], l->array[3]};
  void *expected_list_array[] = {l->array[0], l->array[4], l->array[5]};

  i = 1;

  err = arl_pop_multi(l, i, slice_len, slice);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected_slice, slice,
                              sizeof(expected_slice) / sizeof(void *));
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected_list_array, l->array,
                              sizeof(expected_list_array) / sizeof(void *));
}

void test_arl_pop_multi_success_1(void) {
  size_t slice_len;
  l_error_t err;
  int i;
  arl_ptr l = setup_small_list();

  slice_len = 2;
  void *slice[slice_len];

  void *expected_slice[] = {l->array[2], l->array[3]};
  void *expected_list_array[] = {l->array[0], l->array[1], l->array[4],
                                 l->array[5]};

  i = 2;

  err = arl_pop_multi(l, i, slice_len, slice);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected_slice, slice,
                              sizeof(expected_slice) / sizeof(void *));
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected_list_array, l->array,
                              sizeof(expected_list_array) / sizeof(void *));
}

void test_arl_clear_no_callback(void) {
  l_error_t err;
  arl_ptr l = setup_small_list();
  void *expected_list_array[] = {NULL, NULL, NULL, NULL, NULL, NULL};

  err = arl_clear(l, NULL);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(0, l->length);
  TEST_ASSERT_EQUAL_PTR_ARRAY(expected_list_array, l->array, arl_small_length);
}

void test_arl_clear_callback(void) {
  l_error_t err;
  arl_ptr l = setup_small_list();

  err = arl_clear(l, dummy_free);

  TEST_ASSERT_EQUAL_ERROR(L_SUCCESS, err);
  TEST_ASSERT_EQUAL(0, l->length);
  TEST_ASSERT_EQUAL(arl_small_length, FREE_COUNTER);
}
