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
#include "c_lists/cll_error.h"
#include "cll_interfaces.h"
#include "common/cll_def.h"
#include "utils/cll_overflow_utils.h"
#include "utils/cll_pointers_utils.h"

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

static bool arl_is_i_too_big(arl_ptr l, size_t i);
static void *_arl_get(arl_ptr l, size_t i);
static void _arl_set(arl_ptr l, size_t i, void *value);
static arl_ptr arl_grow_array_capacity(arl_ptr l);
static arl_ptr arl_move_elements_right(arl_ptr l, size_t start_i,
                                       size_t move_by);
static arl_ptr arl_move_elements_left(arl_ptr l, size_t start_i,
                                      size_t move_by);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

size_t arl_length(arl_ptr l) { return l->length; }

/* Returns L_SUCCESS on success. */
/* Behaviour is undefined if `l` is not a valid pointer. */
arl_ptr arl_create(size_t default_capacity) {
  if (cll_is_overflow_size_t_multi(default_capacity, CLL_PTR_SIZE)) {
    errno = CLL_ERROR_OVERFLOW;
    goto ERROR;
  }

  void *l_array = malloc(default_capacity * CLL_PTR_SIZE);

  if (!l_array) {
    goto ERROR_OOM;
  }

  arl_ptr l_local = malloc(sizeof(struct ar_list));

  if (!l_local)
    goto CLEANUP_L_LOCAL_OOM;

  l_local->array = l_array;
  l_local->capacity = default_capacity;
  l_local->length = 0;

  return l_local;

CLEANUP_L_LOCAL_OOM:
  free(l_array);
ERROR_OOM:
  errno = CLL_ERROR_OUT_OF_MEMORY;
ERROR:
  return NULL;
}

void arl_destroy(arl_ptr l) {
  free(l->array);
  free(l);
}

/* Gets pointer to the value under the index.
 * `p` is placeholder for the value's pointer.
 * `l` and `p` have to be valid pointers,
 *  otherwise behaviour is undefined.
 */
void *arl_get(arl_ptr l, size_t i) {
  if (arl_is_i_too_big(l, i)) {
    errno = CLL_ERROR_INDEX_TOO_BIG;
    return NULL;
  }

  return _arl_get(l, i);
}

/* Fills slice with elements from index i till index
 *  i+elemets_amount. Start i and elements amount have
 *  to respect list's length, otherwise error is raised.
 *  Slice's length has to be bigger than elements amount.
 *  Otherwise behaviour is undefined.
 */
void *arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                void *slice[]) {
  size_t k, last_elem_i;

  if (arl_is_i_too_big(l, start_i)) {
    errno = CLL_ERROR_INDEX_TOO_BIG;
    return NULL;
  }

  last_elem_i = start_i + elements_amount;
  if (arl_is_i_too_big(l, last_elem_i)) {
    errno = CLL_ERROR_INVALID_ARGS;
    return NULL;
  }

  for (k = 0; k < elements_amount; k++) {
    slice[k] = _arl_get(l, k + start_i);
  }

  return slice;
}

/* Sets value under the index.
 *  Index has to be smaller than list's length.
 *  `l` and `value` have to be valid pointers,
 *  otherwise behaviour is undefined.
 */
arl_ptr arl_set(arl_ptr l, size_t i, void *value) {
  if (arl_is_i_too_big(l, i)) {
    errno = CLL_ERROR_INDEX_TOO_BIG;
    return NULL;
  }

  _arl_set(l, i, value);

  return l;
}

/* Insert one element under the index.
 *  If index bigger than list's length, appends the value.
 */
arl_ptr arl_insert(arl_ptr l, size_t i, void *value) {
  size_t new_length, move_by = 1;
  void *success;

  if (arl_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  if (new_length > l->capacity) {
    success = arl_grow_array_capacity(l);
    if (!success)
      return NULL;
  }

  success = arl_move_elements_right(l, i, move_by);
  if (!success)
    return NULL;

  _arl_set(l, i, value);

  l->length = new_length;

  return l;
}

/* Insert multiple elements. Better optimized for multiple
 *  inserts than insert. Moving elements is done only once.
 *  i has to be smaller than l->length.
 *  values should hold valid pointers. v_len is values' length.
 */
arl_ptr arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                         void *values[v_len]) {
  size_t new_length, k, move_by = v_len;
  void *success;

  if (arl_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  while (new_length > l->capacity) {
    success = arl_grow_array_capacity(l);
    if (!success)
      return NULL;
  }

  success = arl_move_elements_right(l, i, move_by);
  if (!success)
    return NULL;

  for (k = i; k < i + v_len; k++) {
    _arl_set(l, k, values[k - i]);
  }

  l->length = new_length;

  return l;
}

/* Appends one element to the list's end.
 */
arl_ptr arl_append(arl_ptr l, void *value) {
  return arl_insert(l, l->length + 1, value);
}

/* Pops element from under the index.
 * If list is empty returns NULL and sets
 *   errno to CLL_ERROR_POP_EMPTY_LIST.
 * If i bigger tan list's lenth, substitues it with
 *  maximum poppable i.
 */
void *arl_pop(arl_ptr l, size_t i) {
  const size_t offset = 1;
  void *value_holder;
  void *success;

  if (arl_is_i_too_big(l, i))
    i = l->length - 1;

  if (l->length == 0) {
    errno = CLL_ERROR_POP_EMPTY_LIST;
    return NULL;
  }

  value_holder = _arl_get(l, i);

  success = arl_move_elements_left(l, ++i, offset);
  if (!success)
    return NULL;

  return value_holder;
}

/* Fills holder with elements starting from index i till index
 *  i+elements aomunt. Holder's length has to be bigger than
 *  elements amount. Otherwise behaviour is undefined.
 *  Better optimized for multiple pops than pop. Elements are
 *  moved only once.
 */
void *arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                    void *holder[]) {
  void *success;

  success = arl_slice(l, i, elements_amount, holder);
  if (!success)
    return NULL;

  if (cll_is_overflow_size_t_add(i, elements_amount)) {
    errno = CLL_ERROR_OVERFLOW;
    return NULL;
  }

  success = arl_move_elements_left(l, i + elements_amount, elements_amount);
  if (!success)
    return NULL;

  return holder;
}

/* Removes element from under the index.
 * Executes callback function on removed element,
 *  only if callback is not NULL.
 */
arl_ptr arl_remove(arl_ptr l, size_t i, void (*callback)(void *)) {
  void *p;

  p = arl_pop(l, i);
  if (!p)
    return NULL;

  if (callback)
    callback(p);

  return l;
}

/* Removes all elements from the list.
 * Executes callback function on each removed element,
 *  only if callback is not NULL.
 */
arl_ptr arl_clear(arl_ptr l, void (*callback)(void *)) {
  size_t i;
  void *success;

  // POP MULTI is not used here to avoid extra loop iteration and
  // some memory. Slicing is unnecessary from clear's point of view.
  if (callback) {
    for (i = 0; i < l->length; i++) {
      callback(_arl_get(l, i));
    }
  }

  success = arl_move_elements_left(l, l->length, l->length);
  if (!success)
    return NULL;

  l->length = 0;

  return l;
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/

void *_arl_get(arl_ptr l, size_t i) { return l->array[i]; }
void _arl_set(arl_ptr l, size_t i, void *value) { l->array[i] = value; }

/* Counts list's new capacity.
 * Prevents overflow by assigning ARL_CAPACITY_MAX, whenever overflow
 *   would occur. At some point list will be prevented from growing.
 */
size_t arl_count_new_capacity(size_t current_length, size_t current_capacity) {

  /* Size is always smaller than capacity. There is no need in checking
new_size
   * divided by cur_size overflow.
   */
  if (cll_is_overflow_l_capacity_multi(current_length, 3) ||
      cll_is_overflow_l_capacity_add(current_capacity, 2)) {

    return ARL_CAPACITY_MAX;
  }

  return 3 * current_length / 2 + current_capacity;
}

/* Checks if index is within list boundaries. */
/* The behaviour is undefined if `l` is not a valid pointer. */
bool arl_is_i_too_big(arl_ptr l, size_t i) { return i >= (l->length); }

/* Grows underlaying array. */
arl_ptr arl_grow_array_capacity(arl_ptr l) {
  void *p;
  size_t new_capacity;

  if (l->capacity == ARL_CAPACITY_MAX) {
    errno = CLL_ERROR_REACHED_CAPACITY_MAX;
    goto ERROR;
  }

  new_capacity = arl_count_new_capacity(l->length, l->capacity);

  p = realloc(l->array, new_capacity * CLL_PTR_SIZE);

  if (!p) {
    errno = CLL_ERROR_OUT_OF_MEMORY;
    goto ERROR;
  }

  l->capacity = new_capacity;
  l->array = p;

  return l;

ERROR:
  return NULL;
};

/* Move elements to the right by `move_by`, starting from `start_i`.
 * Set NULL on source value.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, NULL, NULL, 1, 2}
 */
arl_ptr arl_move_elements_right(arl_ptr l, size_t start_i, size_t move_by) {

  size_t new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.
  if (cll_is_overflow_size_t_add(l->length, move_by)) {
    errno = CLL_ERROR_OVERFLOW;
    goto ERROR;
  }

  new_length = l->length + move_by;

  if (new_length > (l->capacity)) {
    errno = CLL_ERROR_INVALID_ARGS;
    goto ERROR;
  }

  if (cll_is_underflow_size_t_sub(l->length, start_i)) {
    errno = CLL_ERROR_UNDERFLOW;
    goto ERROR;
  }

  elements_to_move_amount = l->length - start_i;

  if (elements_to_move_amount == 0)
    goto SUCCESS;

  cll_move_pointers_array_rstart(l->array + start_i + move_by,
                                 l->array + start_i, elements_to_move_amount);

  l->length = new_length;

SUCCESS:
  return l;
ERROR:
  return NULL;
}

/* Move elements to the left by `move_by`, starting from `start_i`.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 2, move_by 1
 *    OUTPUT l.array {1, 2, NULL, ,}
 */
arl_ptr arl_move_elements_left(arl_ptr l, size_t start_i, size_t move_by) {
  /* `move_by` tells how many places we should shift.
   *  Specially usefull when popping slice of array:
   *   `pop elements starting from index 5 till index 8`.
   *  If start_i is bigger or equal to l->length, assumes
   *   end of the list.
   */

  size_t new_length, elements_to_move_amount;
  void **src, **dst;

  // Do not allow reading before list's start.
  if (move_by > start_i)
    move_by = start_i;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.

  if (cll_is_underflow_size_t_sub(l->length, move_by)) {
    errno = CLL_ERROR_UNDERFLOW;
    goto ERROR;
  }

  new_length = l->length - move_by;

  if (cll_is_underflow_size_t_sub(l->length, start_i)) {
    errno = CLL_ERROR_UNDERFLOW;
    goto ERROR;
  }

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

  return l;

ERROR:
  return NULL;
}
