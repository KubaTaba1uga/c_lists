/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// App
#include "ar_list.h"
#include "l_error.h"
#include "utils/overflow_utils.h"
#include "utils/std_lib_utils.h"

/*******************************************************************************
 *    PRIVATE API DECLARATIONS
 ******************************************************************************/
static l_error_t arl_count_new_capacity(size_t current_size,
                                        size_t current_capacity,
                                        size_t *new_capacity);
static l_error_t arl_is_i_too_big(ar_list *l, size_t i, bool *result);
static l_error_t arl_grow_array_capacity(ar_list *l);
static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
                                                 size_t move_by);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* TO-DO */
/* 1. set up errno on failure */

l_error_t arl_init(ar_list *l, size_t default_capacity) {
  /* Function return 0 on success, not 0 on failure. */
  /* `default_capacity` tells maximum amount of pointers
   *     which list can store until first realloc. */
  /*  `default_capacity` is parametrized for flexibility. */
  /*  Which makes You responsible for knowing the data. */

  size_t p_size = get_pointer_size();

  if (is_overflow_size_t_multi(default_capacity, p_size))
    return L_ERROR_OVERFLOW;

  void *arl_array = app_malloc(default_capacity * p_size);

  if (!arl_array)
    return L_ERROR_MEMORY_SHORTAGE;

  l->array = arl_array;
  l->capacity = default_capacity;
  l->size = 0;

  return L_SUCCESS;
}

l_error_t arl_get(ar_list *l, size_t i, void **p) {
  /* Set p to `pointer to value under the index`. */
  bool i_is_invalid;

  arl_is_i_too_big(l, i, &i_is_invalid);

  if (i_is_invalid || !l || !p)
    return L_ERROR_INVALID_ARGS;

  *p = l->array[i];

  return L_SUCCESS;
}

l_error_t arl_set(ar_list *l, size_t i, void *value) {
  /* Set value under the index. */
  /* Index has to be smaller than l.size. */
  /* Do not expand array. */
  bool i_is_invalid;

  if (!l || !value)
    return L_ERROR_INVALID_ARGS;

  arl_is_i_too_big(l, i, &i_is_invalid);

  if (i_is_invalid)
    return L_ERROR_INVALID_ARGS;

  l->array[i] = value;

  return L_SUCCESS;
}

void arl_append(ar_list *l, void *value) {
  /* Insert value to the last possible index. */
  /* If no enough capacity, realloc array. */

  /* size_t last_i = l->size; */

  /* if (arl_is_i_too_big(l, last_i)) */
  arl_grow_array_capacity(l);
}

l_error_t arl_insert(ar_list *l, size_t i, void *value) {
  /* Insert value to the index. */
  /* Return NULL on failure. */
  /* If no enough capacity, realloc array. */
  void *p;
  l_error_t err;
  bool i_is_too_big;

  arl_is_i_too_big(l, i, &i_is_too_big);

  if (i_is_too_big) {
    return L_ERROR_INDEX_TOO_BIG;
  }

  p = arl_move_indexes_by_positive_number(l, i, 1);
  if (!p)
    return L_ERROR_MEMORY_SHORTAGE;

  err = arl_set(l, i, value);
  if (err)
    return err;

  return L_SUCCESS;
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/

// SHRINK IS ONLY IN POP
// IF SIZE < CAPACITY / 3
//     CAPACITY = CAPACITY / 2

static l_error_t arl_count_new_capacity(size_t current_size,
                                        size_t current_capacity,
                                        size_t *new_capacity) {
  /* Useful for array realloc. */

  // TO-DO
  // 1. validate calculations for overflow

  if (!new_capacity)
    return L_ERROR_INVALID_ARGS;

  *new_capacity = (size_t)(3 * current_size / 2 + current_capacity);

  return L_SUCCESS;
}

static l_error_t arl_is_i_too_big(ar_list *l, size_t i, bool *result) {
  if (!result || !l)
    return L_ERROR_INVALID_ARGS;

  *result = i >= (l->size);

  return L_SUCCESS;
}

static l_error_t arl_grow_array_capacity(ar_list *l) {
  void *p;
  size_t new_capacity;
  size_t pointer_size = get_pointer_size();

  if (!l)
    return L_ERROR_INVALID_ARGS;

  arl_count_new_capacity(l->size, l->capacity, &new_capacity);

  if (is_overflow_size_t_multi(new_capacity, pointer_size))
    return L_ERROR_OVERFLOW;

  p = app_realloc(l->array, new_capacity * pointer_size);

  if (!p) {
    return L_ERROR_MEMORY_SHORTAGE;
  }

  l->capacity = new_capacity;
  l->array = p;

  return L_SUCCESS;
};

static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
                                                 size_t move_by) {
  /* Move elements further by `move_by`, starting from `start_i`.
   * Set NULL on source value.  */
  /* Ex: */
  /*    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2 */
  /*    OUTPUT l.array {0, NULL, NULL, 1, 2} */

  // Idea is to detect all failures upfront so recovery from half moved array is
  // not required.

  void *p;

  size_t i, old_size, new_size, i_source, i_dest, elements_to_move_amount;

  old_size = l->size, new_size = l->size + move_by;
  elements_to_move_amount = old_size - start_i;

  // TO-DO
  // set erramount on validation error
  if (new_size > l->capacity)
    return NULL;
  if (elements_to_move_amount == 0)
    return NULL;

  l->size = new_size;

  start_i--;

  for (i = elements_to_move_amount; i > 0; i--) {
    i_source = start_i + i;
    i_dest = i_source + move_by;

    arl_get(l, i_source, &p);

    arl_set(l, i_dest, p);

    arl_set(l, i_source, NULL);
  }

  return l->array;
}
