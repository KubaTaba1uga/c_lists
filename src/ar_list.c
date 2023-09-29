/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/* Array list require allocating one continous chunk of memory. As all
 * allocating functions take required memory size in size_t type, it is safe to
 * assume that SIZE_MAX is maximum allocable memory size for one object.
 * To not shrink list accadidentially at some point ARL_MAX_CAPACITY
 * is defined as upper boundary. If You require bigger capacity check out other
 * lists.
 */

// TO-DO pop - remove and return one item (by default last, do not shrink)
// TO-DO remove - remove one item (index must be specified, do not shrink)
// TO-DO clear - remove all items from a list (do not shrink)
// TO-DO insert_multi - same as insert but array of elements (instead of single
// element)
// TO-DO extend - join two lists into one

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// App
#include "ar_list.h"
#include "l_def.h"
#include "l_error.h"

#include "interfaces/std_lib_interface.h"
#include "utils/overflow_utils.h"
#include "utils/pointers_utils.h"

/*******************************************************************************
 *    PRIVATE API DECLARATIONS
 ******************************************************************************/
struct ar_list {
  /* Number of elements.*/
  size_t length;

  /* Maximum number of elements. */
  size_t capacity;

  /* Storage. */
  void **array;
};

/* static size_t arl_count_new_capacity(size_t current_size, */
/* size_t current_capacity); */
static bool arl_is_i_too_big(arl_ptr l, size_t i);
static void _arl_get(arl_ptr l, size_t i, void **p);
static void _arl_set(arl_ptr l, size_t i, void *value);
static l_error_t arl_grow_array_capacity(arl_ptr l);
static l_error_t arl_move_elements_right(arl_ptr l, size_t start_i,
                                         size_t move_by);
/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

void arl_length(arl_ptr l, size_t *p) { *p = l->length; }

/* Returns L_SUCCESS on success. */
/* Behaviour is undefined if `l` is not a valid pointer. */
l_error_t arl_create(arl_ptr *l, size_t default_capacity) {
  if (is_overflow_size_t_multi(default_capacity, L_PTR_SIZE))
    return L_ERROR_OVERFLOW;

  void *l_array = app_malloc(default_capacity * L_PTR_SIZE);

  if (!l_array)
    return L_ERROR_OUT_OF_MEMORY;

  arl_ptr l_local = app_malloc(sizeof(struct ar_list));

  if (!l_local)
    goto CLEANUP_L_LOCAL_OOM;

  l_local->array = l_array;
  l_local->capacity = default_capacity;
  l_local->length = 0;
  *l = l_local;

  return L_SUCCESS;

CLEANUP_L_LOCAL_OOM:
  free(l_array);
  return L_ERROR_OUT_OF_MEMORY;
}

/* Gets pointer to the value under the index.
 * `p` is placeholder for the value's pointer.
 * `l` and `p` have to be valid pointers,
 *  otherwise behaviour is undefined.
 */
l_error_t arl_get(arl_ptr l, size_t i, void **p) {
  if (arl_is_i_too_big(l, i))
    return L_ERROR_INDEX_TOO_BIG;

  _arl_get(l, i, p);

  return L_SUCCESS;
}

/* Sets value under the index.
 *  Index has to be smaller than list's length.
 *  `l` and `value` have to be valid pointers,
 *  otherwise behaviour is undefined.
 */
l_error_t arl_set(arl_ptr l, size_t i, void *value) {
  if (arl_is_i_too_big(l, i))
    return L_ERROR_INDEX_TOO_BIG;

  _arl_set(l, i, value);

  return L_SUCCESS;
}

l_error_t arl_insert(arl_ptr l, size_t i, void *value) {
  size_t new_length, move_by = 1;
  l_error_t err;

  if (arl_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  if (new_length > l->capacity) {
    err = arl_grow_array_capacity(l);
    if (err)
      return err;
  }

  err = arl_move_elements_right(l, i, move_by);
  if (err)
    return err;

  _arl_set(l, i, value);

  l->length = new_length;

  return L_SUCCESS;
}

l_error_t arl_append(arl_ptr l, void *value) {
  return arl_insert(l, l->length + 1, value);
}

l_error_t arl_pop(arl_ptr l, size_t i, void *value) {
  return arl_insert(l, l->length + 1, value);
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/

void _arl_get(arl_ptr l, size_t i, void **p) { *p = l->array[i]; }
void _arl_set(arl_ptr l, size_t i, void *value) { l->array[i] = value; }

// SHRINK ONLY IN POP
// IF SIZE < CAPACITY / 3
//     CAPACITY = CAPACITY / 2

/* Counts list's new capacity.
 * The behaviour is undefined if new capacity is not a valid pointer.
 * Prevents overflow by assigning L_MAX_SIZE_CAPACITY, whenever overflow
 *   would occur. At some point list will be prevented from growing.
 */
size_t arl_count_new_capacity(size_t current_length, size_t current_capacity) {

  /* Size is always smaller than capacity. There is no need in checking
new_size
   * divided by cur_size overflow.
   */
  if (is_overflow_l_capacity_multi(current_length, 3) ||
      is_overflow_l_capacity_add(current_capacity, 2)) {

    return ARL_CAPACITY_MAX;
  }

  return 3 * current_length / 2 + current_capacity;
}

/* Checks if index is within list boundaries. */
/* The behaviour is undefined if `l` is not a valid pointer. */
bool arl_is_i_too_big(arl_ptr l, size_t i) { return i >= (l->length); }

/* Grows underlaying array. */
static l_error_t arl_grow_array_capacity(arl_ptr l) {
  void *p;
  size_t new_capacity;

  if (l->capacity == ARL_CAPACITY_MAX)
    return L_ERROR_REACHED_CAPACITY_MAX;

  new_capacity = arl_count_new_capacity(l->length, l->capacity);

  p = app_realloc(l->array, new_capacity * L_PTR_SIZE);
  if (!p) {
    return L_ERROR_OUT_OF_MEMORY;
  }

  l->capacity = new_capacity;
  l->array = p;

  return L_SUCCESS;
};

/* Move elements to the right by `move_by`, starting from `start_i`.
 * Set NULL on source value.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, NULL, NULL, 1, 2}
 */
l_error_t arl_move_elements_right(arl_ptr l, size_t start_i, size_t move_by) {

  size_t old_length, new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.
  if (is_overflow_size_t_add(l->length, move_by))
    return L_ERROR_OVERFLOW;

  old_length = l->length, new_length = l->length + move_by;

  if (new_length > (l->capacity))
    return L_ERROR_INVALID_ARGS;

  if (is_underflow_size_t_sub(old_length, start_i))
    return L_ERROR_OVERFLOW;

  elements_to_move_amount = old_length - start_i;

  // this check can be unnecessary
  if (elements_to_move_amount == 0)
    return L_SUCCESS;

  move_pointers_array_rstart(l->array + start_i + move_by, l->array + start_i,
                             elements_to_move_amount);

  l->length = new_length;

  return L_SUCCESS;
}

/* Move elements to the left by `move_by`, starting from `start_i`.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 2, move_by 1
 *    OUTPUT l.array {1, 2, , , ,}
 */
l_error_t arl_move_elements_left(arl_ptr l, size_t start_i, size_t move_by) {

  size_t old_length, new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.
  if (is_underflow_size_t_sub(l->length, move_by))
    return L_ERROR_OVERFLOW;

  old_length = l->length, new_length = l->length - move_by;

  if (is_underflow_size_t_sub(old_length, new_length))
    return L_ERROR_OVERFLOW;

  elements_to_move_amount = old_length - new_length;

  // this check can be unnecessary
  if (elements_to_move_amount == 0)
    return L_SUCCESS;

  move_pointers_array_rstart(l->array + start_i, l->array + start_i + move_by,
                             elements_to_move_amount);

  // Shrink array when required
  /* if (new_length > (l->capacity)) */
  /* return L_ERROR_INVALID_ARGS; */

  l->length = new_length;
  return L_SUCCESS;
}
