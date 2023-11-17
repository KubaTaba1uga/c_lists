/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/* Array list require allocating one continous chunk of memory. As all
 * allocating functions take required memory size in size_t type, it is safe to
 * assume that SIZE_MAX is maximum allocable memory size for a list's instance.
 * To not shrink list accadidentially at some point ARL_MAX_CAPACITY
 * is defined as upper boundary. If You require bigger capacity check out other
 * lists.
 * List's array is not being shrinked, even on pop or clear. As it may be
 * usefull for someone, it is not essential to list's logic (in opposition
 * to growing). I would rather do some versions of current functions which
 * would shrink internall array, than edit current ones. Maybe even new list?
 */

// TO-DO extend - join two lists into one
// TO-DO shrink array:
// 1. pop
// 2. remove
// 3. pop multi
// 4. clear - get as arg new capacity, shrink to new capacity.
//     ?? do we really need it? I think this is duplication of
//     destroy currtent list, create a new one. ??

// TO-DO use opaque pointers to allow user link to struct
// TO-DO make library as meson's custom target

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// App
#include "c_lists/ar_list.h"
#include "c_lists/cll_def.h"
#include "c_lists/cll_error.h"
#include "cll_interfaces.h"
#include "utils/cll_overflow_utils.h"
#include "utils/cll_pointers_utils.h"

/*******************************************************************************
 *    PRIVATE DECLARATIONS
 ******************************************************************************/
struct ar_list {
  /* Number of elements.*/
  size_t length;

  /* Maximum number of elements. */
  size_t capacity;

  /* Storage. */
  CLL_VALUE_TYPE *array;
};

static bool arl_is_i_too_big(arl_ptr l, size_t i);
static void _arl_get(arl_ptr l, size_t i, CLL_VALUE_TYPE *value);
static void _arl_set(arl_ptr l, size_t i, CLL_VALUE_TYPE value);

static cll_error arl_grow_array_capacity(arl_ptr l);
static cll_error arl_move_elements_right(arl_ptr l, size_t start_i,
                                         size_t move_by);
static cll_error arl_move_elements_left(arl_ptr l, size_t start_i,
                                        size_t move_by);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* Returns list's length. */
cll_error arl_length(arl_ptr l, size_t *length) {
  *length = l->length;

  return CLL_SUCCESS;
}

/* Creates array list's instance. Returns NULL and sets
 *  errno on failure.
 * Behaviour is undefined if `default_capacity` is equal 0.
 */
cll_error arl_create(arl_ptr *l, size_t default_capacity) {
  if (cll_is_overflow_size_t_multi(default_capacity, CLL_VALUE_SIZE))
    return CLL_ERROR_OVERFLOW;

  void *l_array = malloc(default_capacity * CLL_VALUE_SIZE);

  if (!l_array)
    goto ERROR_OOM;

  arl_ptr l_local = malloc(sizeof(struct ar_list));

  if (!l_local)
    goto CLEANUP_L_LOCAL_OOM;

  l_local->array = (CLL_VALUE_TYPE *)l_array;
  l_local->capacity = default_capacity;
  l_local->length = 0;

  *l = l_local;

  return CLL_SUCCESS;

CLEANUP_L_LOCAL_OOM:
  free(l_array);
ERROR_OOM:
  return CLL_ERROR_OUT_OF_MEMORY;
}

/* Frees resouces allocated for array list's instance. */
cll_error arl_destroy(arl_ptr l) {
  free(l->array);
  free(l);

  return CLL_SUCCESS;
}

/* Gets the value under the index.
 * Returns 0 and sets errno on failure.
 * On success returns value and doesn't set errno.
 */
cll_error arl_get(arl_ptr l, size_t i, CLL_VALUE_TYPE *value) {
  if (arl_is_i_too_big(l, i))
    return CLL_ERROR_INDEX_TOO_BIG;

  _arl_get(l, i, value);

  return CLL_SUCCESS;
}

/* Fills slice with elements from index i till index
 *  i+elemets_amount. Start i and elements amount have
 *  to respect list's length, otherwise error is raised.
 *  Slice's length has to be bigger than elements amount.
 *  Otherwise behaviour is undefined.
 */
cll_error arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                    CLL_VALUE_TYPE slice[]) {
  size_t k, last_elem_i;

  if (arl_is_i_too_big(l, start_i))
    return CLL_ERROR_INDEX_TOO_BIG;

  last_elem_i = start_i + elements_amount;
  if (arl_is_i_too_big(l, last_elem_i))
    return CLL_ERROR_INVALID_ARGS;

  for (k = 0; k < elements_amount; k++) {
    _arl_get(l, k + start_i, &slice[k]);
  }

  return CLL_SUCCESS;
}

/* Sets value under the index.
 * Index has to be smaller than list's length.
 * Returns NULL and sets errno on failure.
 */
cll_error arl_set(arl_ptr l, size_t i, CLL_VALUE_TYPE value) {
  if (arl_is_i_too_big(l, i))
    return CLL_ERROR_INDEX_TOO_BIG;

  _arl_set(l, i, value);

  return CLL_SUCCESS;
}

/* Insert one element under the index.
 *  If index bigger than list's length, appends the value.
 */
cll_error arl_insert(arl_ptr l, size_t i, CLL_VALUE_TYPE value) {
  size_t new_length, move_by = 1;
  cll_error err;

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

  return CLL_SUCCESS;
}

/* Appends one element to the list's end.
 */
cll_error arl_append(arl_ptr l, CLL_VALUE_TYPE value) {
  return arl_insert(l, l->length + 1, value);
}

/* Insert multiple elements. Better optimized for multiple
 *  inserts than insert. Moving elements is done only once.
 *  i has to be smaller than l->length.
 *  values should hold valid pointers. v_len is values' length.
 */
cll_error arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                           CLL_VALUE_TYPE values[v_len]) {
  size_t new_length, k, move_by = v_len;
  cll_error err;

  if (arl_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  while (new_length > l->capacity) {
    err = arl_grow_array_capacity(l);
    if (err)
      return err;
  }

  err = arl_move_elements_right(l, i, move_by);
  if (err)
    return err;

  for (k = i; k < i + v_len; k++) {
    _arl_set(l, k, values[k - i]);
  }

  l->length = new_length;

  return CLL_SUCCESS;
}

/* Pops element from under the index. Sets
 * value to the popped element's value.
 * If list is empty, returns CLL_ERROR_POP_EMPTY_LIST.
 * If i bigger tan list's lenth, substitues it with
 *  maximum poppable i.
 */
cll_error arl_pop(arl_ptr l, size_t i, CLL_VALUE_TYPE *value) {
  const size_t offset = 1;
  void *value_holder;
  cll_error err;

  if (arl_is_i_too_big(l, i))
    i = l->length - 1;
  if (l->length == 0) {
    return CLL_ERROR_POP_EMPTY_LIST;
  }

  _arl_get(l, i, &value_holder);

  err = arl_move_elements_left(l, ++i, offset);
  if (err)
    return err;

  *value = value_holder;

  return CLL_SUCCESS;
}

/* Fills holder with elements starting from index i till index
 *  i+elements aomunt. Holder's length has to be bigger than
 *  elements amount. Otherwise behaviour is undefined.
 *  Better optimized for multiple pops than pop. Elements are
 *  moved only once.
 */
cll_error arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                        CLL_VALUE_TYPE holder[]) {
  cll_error err;

  err = arl_slice(l, i, elements_amount, holder);
  if (err)
    return err;

  if (cll_is_overflow_size_t_add(i, elements_amount))
    return CLL_ERROR_OVERFLOW;

  err = arl_move_elements_left(l, i + elements_amount, elements_amount);
  if (err)
    return err;

  return CLL_SUCCESS;
}

/* Removes element from under the index.
 * Executes callback function on removed element,
 *  only if callback is not NULL.
 */
cll_error arl_remove(arl_ptr l, size_t i, void (*callback)(CLL_VALUE_TYPE)) {
  void *p;
  cll_error err;

  err = arl_pop(l, i, &p);
  if (err)
    return err;

  if (callback)
    callback(p);

  return CLL_SUCCESS;
}

/* Removes all elements from the list.
 * Executes callback function on each removed element,
 *  only if callback is not NULL.
 */
cll_error arl_clear(arl_ptr l, void (*callback)(CLL_VALUE_TYPE)) {
  size_t i;
  CLL_VALUE_TYPE value;
  cll_error err;

  // POP MULTI is not used here to avoid extra loop iteration and
  // some memory. Slicing is unnecessary from clear's point of view.
  if (callback) {
    for (i = 0; i < l->length; i++) {
      _arl_get(l, i, &value);
      callback(value);
    }
  }

  err = arl_move_elements_left(l, l->length, l->length);
  if (err)
    return err;

  l->length = 0;

  return CLL_SUCCESS;
}

/* Move elements to the right by `move_by`, starting from `start_i`.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, , , 1, 2}
 */
cll_error arl_move_elements_right(arl_ptr l, size_t start_i, size_t move_by) {

  size_t new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.
  if (cll_is_overflow_size_t_add(l->length, move_by))
    return CLL_ERROR_OVERFLOW;

  new_length = l->length + move_by;

  if (new_length > (l->capacity))
    return CLL_ERROR_INVALID_ARGS;

  if (cll_is_underflow_size_t_sub(l->length, start_i))
    return CLL_ERROR_UNDERFLOW;

  elements_to_move_amount = l->length - start_i;

  if (elements_to_move_amount == 0)
    return CLL_SUCCESS;

  cll_move_pointers_array_rstart(l->array + start_i + move_by,
                                 l->array + start_i, elements_to_move_amount);

  l->length = new_length;

  return CLL_SUCCESS;
}

/* Move elements to the left by `move_by`, starting from start_i
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 2, move_by 1
 *    OUTPUT l.array {1, 2, , ,}
 */
cll_error arl_move_elements_left(arl_ptr l, size_t start_i, size_t move_by) {
  /* ove_byop elements starting from index 5 till index 8
   *  If start_i is bigger or equal to l->length, assumes
   *   end of the list.
   */

  size_t new_length, elements_to_move_amount;
  CLL_VALUE_TYPE *src, *dst;

  // Do not allow reading before list's start.
  if (move_by > start_i)
    move_by = start_i;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.

  if (cll_is_underflow_size_t_sub(l->length, move_by))
    return CLL_ERROR_UNDERFLOW;

  new_length = l->length - move_by;

  if (cll_is_underflow_size_t_sub(l->length, start_i))
    return CLL_ERROR_UNDERFLOW;

  elements_to_move_amount = l->length - start_i;

  if (start_i < l->length) {
    src = l->array + start_i;
    dst = src - move_by;
  } else {
    // Allow deleting last element.
    // TO-DO too hackish, find cleaner solution
    start_i = l->length;
    src = dst = l->array + start_i - move_by;
    elements_to_move_amount = move_by;
  }

  cll_move_pointers_array_lstart(dst, src, elements_to_move_amount);

  l->length = new_length;

  return CLL_SUCCESS;
}

void _arl_get(arl_ptr l, size_t i, CLL_VALUE_TYPE *value) {
  *value = l->array[i];
}
void _arl_set(arl_ptr l, size_t i, CLL_VALUE_TYPE value) {
  l->array[i] = value;
}

/* Checks if index is within list boundaries.
 * The behaviour is undefined if is not a valid pointer.
 */
bool arl_is_i_too_big(arl_ptr l, size_t i) { return i >= (l->length); }

/* Counts list's new capacity.
 */
cll_error arl_count_new_capacity(size_t current_length, size_t current_capacity,
                                 size_t *new_capacity) {

  /* Size is always smaller than capacity. There is no need in checking new_size
   * divided by cur_size overflow.
   */
  if (cll_is_overflow_l_capacity_multi(current_length, 3) ||
      cll_is_overflow_l_capacity_add(current_capacity, 2)) {

    return CLL_ERROR_OVERFLOW;
  }

  *new_capacity = 3 * current_length / 2 + current_capacity;

  return CLL_SUCCESS;
}

/* Grows underlaying array. */
cll_error arl_grow_array_capacity(arl_ptr l) {
  void *p;
  size_t new_capacity;
  cll_error err;

  err = arl_count_new_capacity(l->length, l->capacity, &new_capacity);
  if (err)
    return err;

  p = realloc(l->array, new_capacity * CLL_VALUE_SIZE);
  if (!p) {
    return CLL_ERROR_OUT_OF_MEMORY;
  }

  l->capacity = new_capacity;
  l->array = p;

  return CLL_SUCCESS;
};
