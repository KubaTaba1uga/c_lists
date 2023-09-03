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
#include "utils/overflow_utils.h"
#include "utils/std_lib_utils.h"

/*******************************************************************************
 *    PRIVATE API DECLARATIONS
 ******************************************************************************/
static size_t arl_count_new_capacity(size_t size, size_t capacity);
static bool arl_is_i_invalid(ar_list *l, size_t i);
static int arl_grow_array_capacity(ar_list *l);
static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
                                                 size_t move_by);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* TO-DO */
/* 1. set up errno on failure */

int arl_init(ar_list *l, size_t default_capacity) {
  /* Function return 0 on success, not 0 on failure. */
  /* `default_capacity` tells maximum amount of pointers
   *     which list can store until first realloc. */
  /*  `default_capacity` is parametrized for flexibility. */
  /*  Which makes You responsible for knowing the data. */

  size_t p_size = get_pointer_size();

  if (!is_overflow_size_t_multi(default_capacity, p_size))
    return -1;

  void *arl_array = app_malloc(default_capacity * p_size);

  if (!arl_array)
    return -2;

  l->array = arl_array;
  l->capacity = default_capacity;
  l->size = 0;

  return 0;
}

void *arl_get(ar_list *l, size_t i) {
  /* Get value under the index. */
  /* Return NULL on failure. */

  if (arl_is_i_invalid(l, i))
    return NULL;

  return l->array[i];
}

void *arl_set(ar_list *l, size_t i, void *value) {
  /* Set value under the index. */
  /* Index has to be smaller than l.size. */
  /* Do not expand array. */
  /* Return value on success. */
  /* Return NULL on failure. */

  if (arl_is_i_invalid(l, i))
    return NULL;

  l->array[i] = value;

  return l->array[i];
}

/* void *arl_append(ar_list *l, void *value) { */
/*   /\* Insert value to the last possible index. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */

/*   size_t last_i = l->size; */

/*   if (arl_is_i_invalid(l, last_i)) */
/*     arl_grow_array_capacity(l); */
/* } */

// to-do: change void* to int
void *arl_insert(ar_list *l, size_t i, void *value) {
  /* Insert value to the index. */
  /* Return NULL on failure. */
  /* If no enough capacity, realloc array. */
  void *p;

  if (arl_is_i_invalid(l, i))
    arl_grow_array_capacity(l);

  p = arl_move_indexes_by_positive_number(l, i, 1);
  if (!p)
    return NULL;

  p = arl_set(l, i, value);

  return p;
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/

// SHRINK IS ONLY IN POP
// IF SIZE < CAPACITY / 3
//     CAPACITY = CAPACITY / 2

static size_t arl_count_new_capacity(size_t size, size_t capacity) {
  /* Useful for array realloc. */
  return (size_t)(3 * size / 2 + capacity);
}

static bool arl_is_i_invalid(ar_list *l, size_t i) {
  /* Return true if i is not in l->array boundaries. */
  return i >= (l->size);
}

static int arl_grow_array_capacity(ar_list *l) {
  void *p;
  size_t new_capacity = arl_count_new_capacity(l->size, l->capacity);
  size_t pointer_size = get_pointer_size();

  if (is_overflow_size_t_multi(new_capacity, pointer_size))
    return -1;

  p = app_realloc(l->array, new_capacity * pointer_size);

  if (!p) {
    return -2;
  }

  l->capacity = new_capacity;
  l->array = p;

  return 0;
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

    p = arl_get(l, i_source);

    p = arl_set(l, i_dest, p);

    arl_set(l, i_source, NULL);
  }

  return l->array;
}
