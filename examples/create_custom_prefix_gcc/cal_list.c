/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/* Array list require allocating one continous chunk of memory. As all
 * allocating functions take required memory size in size_t type, it is safe to
 * assume that SIZE_MAX is maximum allocable memory size for a list's instance.
 * To not shrink list accadidentially at some point CAL_MAX_CAPACITY
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
#include "cal_list.h"
#ifdef ENABLE_TESTS
#include "cll_interfaces.h"
#endif

/*******************************************************************************
 *    PRIVATE DECLARATIONS
 ******************************************************************************/
struct cal_def {
  /* Number of elements.*/
  size_t length;

  /* Maximum number of elements. */
  size_t capacity;

  /* Storage. */
  CAL_VALUE_TYPE *array;
};

static bool _is_i_too_big(cal_ptr l, size_t i);
static void _get(cal_ptr l, size_t i, CAL_VALUE_TYPE *value);
static void _set(cal_ptr l, size_t i, CAL_VALUE_TYPE value);
static cal_error _grow_array_capacity(cal_ptr l);
static cal_error _move_elements_right(cal_ptr l, size_t start_i,
                                      size_t move_by);
static cal_error _move_elements_left(cal_ptr l, size_t start_i, size_t move_by);
// Pointers utils
static bool _is_overflow_size_t_multi(size_t a, size_t b);
static bool _is_overflow_size_t_add(size_t a, size_t b);
static bool _is_underflow_size_t_sub(size_t a, size_t b);
// Array utils
static void _move_array_elements_rstart(CAL_VALUE_TYPE dest[],
                                        CAL_VALUE_TYPE src[], size_t n);
static void _move_array_elements_lstart(CAL_VALUE_TYPE dest[],
                                        CAL_VALUE_TYPE src[], size_t n);
// Error utils
static const char *const CAL_ERROR_STRINGS[] = {
    // 0
    "Success",
    // 1
    "Invalid arguments",
    // 2
    "Overflow",
    // 3
    "Underflow",
    // 4
    "Not enough memory",
    // 5
    "Index too big",
    // 6
    "Array's maximum capacity reached",
    // 7
    "Popping empty list is disallowed",

};

static const size_t CAL_ERROR_STRINGS_LEN =
    sizeof(CAL_ERROR_STRINGS) / sizeof(char *);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* Creates array list's instance.
 * Behaviour is undefined if `default_capacity` is equal 0.
 */
cal_error cal_create(cal_ptr *l, size_t default_capacity) {
  if (_is_overflow_size_t_multi(default_capacity, CAL_VALUE_SIZE))
    return CAL_ERROR_OVERFLOW;

  void *l_array = malloc(default_capacity * CAL_VALUE_SIZE);

  if (!l_array)
    goto ERROR_OOM;

  cal_ptr l_local = malloc(sizeof(struct cal_def));

  if (!l_local)
    goto CLEANUP_L_LOCAL_OOM;

  l_local->array = (CAL_VALUE_TYPE *)l_array;
  l_local->capacity = default_capacity;
  l_local->length = 0;

  *l = l_local;

  return CAL_SUCCESS;

CLEANUP_L_LOCAL_OOM:
  free(l_array);
ERROR_OOM:
  return CAL_ERROR_OUT_OF_MEMORY;
}

/* Frees resouces allocated for array list's instance.
 */
cal_error cal_destroy(cal_ptr l) {
  free(l->array);
  free(l);

  return CAL_SUCCESS;
}

/* Gets list's length.
 */
cal_error cal_length(cal_ptr l, size_t *length) {
  *length = l->length;

  return CAL_SUCCESS;
}

/* Gets value under the index.
 */
cal_error cal_get(cal_ptr l, size_t i, CAL_VALUE_TYPE *value) {
  if (_is_i_too_big(l, i))
    return CAL_ERROR_INDEX_TOO_BIG;

  _get(l, i, value);

  return CAL_SUCCESS;
}

/* Fills slice with elements from index i till index
 *  i+elemets_amount. Start i and elements amount have
 *  to respect list's length, otherwise error is raised.
 *  Slice's length has to be bigger than elements amount.
 *  Otherwise behaviour is undefined.
 */
cal_error cal_slice(cal_ptr l, size_t start_i, size_t elements_amount,
                    CAL_VALUE_TYPE slice[]) {
  size_t k, last_elem_i;

  if (_is_i_too_big(l, start_i))
    return CAL_ERROR_INDEX_TOO_BIG;

  last_elem_i = start_i + elements_amount;
  if (_is_i_too_big(l, last_elem_i))
    return CAL_ERROR_INVALID_ARGS;

  for (k = 0; k < elements_amount; k++) {
    _get(l, k + start_i, &slice[k]);
  }

  return CAL_SUCCESS;
}

/* Sets value under the index.
 * Index has to be smaller than list's length.
 * Returns NULL and sets errno on failure.
 */
cal_error cal_set(cal_ptr l, size_t i, CAL_VALUE_TYPE value) {
  if (_is_i_too_big(l, i))
    return CAL_ERROR_INDEX_TOO_BIG;

  _set(l, i, value);

  return CAL_SUCCESS;
}

/* Insert one element under the index.
 *  If index bigger than list's length, appends the value.
 */
cal_error cal_insert(cal_ptr l, size_t i, CAL_VALUE_TYPE value) {
  size_t new_length, move_by = 1;
  cal_error err;

  if (_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  if (new_length > l->capacity) {
    err = _grow_array_capacity(l);
    if (err)
      return err;
  }

  err = _move_elements_right(l, i, move_by);
  if (err)
    return err;

  _set(l, i, value);

  l->length = new_length;

  return CAL_SUCCESS;
}

/* Appends one element to the list's end.
 */
cal_error cal_append(cal_ptr l, CAL_VALUE_TYPE value) {
  return cal_insert(l, l->length + 1, value);
}

/* Insert multiple elements. Better optimized for multiple
 *  inserts than insert. Moving elements is done only once.
 *  i has to be smaller than l->length.
 *  values should hold valid pointers. v_len is values' length.
 */
cal_error cal_insert_multi(cal_ptr l, size_t i, size_t v_len,
                           CAL_VALUE_TYPE values[v_len]) {
  size_t new_length, k, move_by = v_len;
  cal_error err;

  if (_is_i_too_big(l, i))
    i = l->length;

  new_length = l->length + move_by;

  while (new_length > l->capacity) {
    err = _grow_array_capacity(l);
    if (err)
      return err;
  }

  err = _move_elements_right(l, i, move_by);
  if (err)
    return err;

  for (k = i; k < i + v_len; k++) {
    _set(l, k, values[k - i]);
  }

  l->length = new_length;

  return CAL_SUCCESS;
}

/* Pops element from under the index. Sets
 * value to the popped element's value.
 * If list is empty, returns CAL_ERROR_POP_EMPTY_LIST.
 * If i bigger tan list's lenth, substitues it with
 *  maximum poppable i.
 */
cal_error cal_pop(cal_ptr l, size_t i, CAL_VALUE_TYPE *value) {
  const size_t offset = 1;
  CAL_VALUE_TYPE value_holder;
  cal_error err;

  if (_is_i_too_big(l, i))
    i = l->length - 1;
  if (l->length == 0) {
    return CAL_ERROR_POP_EMPTY_LIST;
  }

  _get(l, i, &value_holder);

  err = _move_elements_left(l, ++i, offset);
  if (err)
    return err;

  *value = value_holder;

  return CAL_SUCCESS;
}

/* Fills holder with elements starting from index i till index
 *  i+elements aomunt. Holder's length has to be bigger than
 *  elements amount. Otherwise behaviour is undefined.
 *  Better optimized for multiple pops than pop. Elements are
 *  moved only once.
 */
cal_error cal_pop_multi(cal_ptr l, size_t i, size_t elements_amount,
                        CAL_VALUE_TYPE holder[]) {
  cal_error err;

  err = cal_slice(l, i, elements_amount, holder);
  if (err)
    return err;

  if (_is_overflow_size_t_add(i, elements_amount))
    return CAL_ERROR_OVERFLOW;

  err = _move_elements_left(l, i + elements_amount, elements_amount);
  if (err)
    return err;

  return CAL_SUCCESS;
}

/* Removes element from under the index.
 * Executes callback function on removed element,
 *  only if callback is not NULL.
 */
cal_error cal_remove(cal_ptr l, size_t i, void (*callback)(CAL_VALUE_TYPE)) {
  CAL_VALUE_TYPE p;
  cal_error err;

  err = cal_pop(l, i, &p);
  if (err)
    return err;

  if (callback)
    callback(p);

  return CAL_SUCCESS;
}

/* Removes all elements from the list.
 * Executes callback function on each removed element,
 *  only if callback is not NULL.
 */
cal_error cal_clear(cal_ptr l, void (*callback)(CAL_VALUE_TYPE)) {
  size_t i;
  CAL_VALUE_TYPE value;
  cal_error err;

  // POP MULTI is not used here to avoid extra loop iteration and
  // some memory. Slicing is unnecessary from clear's point of view.
  if (callback) {
    for (i = 0; i < l->length; i++) {
      _get(l, i, &value);
      callback(value);
    }
  }

  err = _move_elements_left(l, l->length, l->length);
  if (err)
    return err;

  l->length = 0;

  return CAL_SUCCESS;
}

/*******************************************************************************
 *    ERRORS UTILS
 ******************************************************************************/

const char *cal_strerror(cal_error error) {
  // Return string on success, NULL on failure.
  // This function is only exception from `always
  //  return cal_error_t` design decision.
  // Idea is to mimic org strerror.

  if ( // Upper bound
      (error >= CAL_ERROR_LEN) || (error >= CAL_ERROR_STRINGS_LEN) ||
      // Lower bound
      (error < 0))
    return NULL;

  return CAL_ERROR_STRINGS[error];
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
void _get(cal_ptr l, size_t i, CAL_VALUE_TYPE *value) { *value = l->array[i]; }
void _set(cal_ptr l, size_t i, CAL_VALUE_TYPE value) { l->array[i] = value; }

/* Checks if index is within list boundaries.
 * The behaviour is undefined if is not a valid pointer.
 */
bool _is_i_too_big(cal_ptr l, size_t i) { return i >= (l->length); }

/* Counts list's new capacity.
 */
cal_error _count_new_capacity(size_t current_length, size_t current_capacity,
                              size_t *new_capacity) {

  /* Size is always smaller than capacity. There is no need in checking
   * new_size divided by cur_size overflow.
   */
  if (_is_overflow_size_t_multi(current_length, 3) ||
      _is_overflow_size_t_add(current_capacity, 2)) {

    return CAL_ERROR_OVERFLOW;
  }

  *new_capacity = 3 * current_length / 2 + current_capacity;

  return CAL_SUCCESS;
}

/* Grows underlaying array. */
cal_error _grow_array_capacity(cal_ptr l) {
  void *p;
  size_t new_capacity;
  cal_error err;

  err = _count_new_capacity(l->length, l->capacity, &new_capacity);
  if (err)
    return err;

  p = realloc(l->array, new_capacity * CAL_VALUE_SIZE);
  if (!p) {
    return CAL_ERROR_OUT_OF_MEMORY;
  }

  l->capacity = new_capacity;
  l->array = p;

  return CAL_SUCCESS;
};

/* Move elements to the right by `move_by`, starting from `start_i`.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, , , 1, 2}
 */
cal_error _move_elements_right(cal_ptr l, size_t start_i, size_t move_by) {

  size_t new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved
  // array
  //  is not required.
  if (_is_overflow_size_t_add(l->length, move_by))
    return CAL_ERROR_OVERFLOW;

  new_length = l->length + move_by;

  if (new_length > (l->capacity))
    return CAL_ERROR_INVALID_ARGS;

  if (_is_underflow_size_t_sub(l->length, start_i))
    return CAL_ERROR_UNDERFLOW;

  elements_to_move_amount = l->length - start_i;

  if (elements_to_move_amount == 0)
    return CAL_SUCCESS;

  _move_array_elements_rstart(l->array + start_i + move_by, l->array + start_i,
                              elements_to_move_amount);

  l->length = new_length;

  return CAL_SUCCESS;
}

/* Move elements to the left by `move_by`, starting from start_i
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 2, move_by 1
 *    OUTPUT l.array {1, 2, , ,}
 */
cal_error _move_elements_left(cal_ptr l, size_t start_i, size_t move_by) {
  /* ove_byop elements starting from index 5 till index 8
   *  If start_i is bigger or equal to l->length, assumes
   *   end of the list.
   */

  size_t new_length, elements_to_move_amount;
  CAL_VALUE_TYPE *src, *dst;

  // Do not allow reading before list's start.
  if (move_by > start_i)
    move_by = start_i;

  // Idea is to detect all failures upfront so recovery from half moved
  // array
  //  is not required.

  if (_is_underflow_size_t_sub(l->length, move_by))
    return CAL_ERROR_UNDERFLOW;

  new_length = l->length - move_by;

  if (_is_underflow_size_t_sub(l->length, start_i))
    return CAL_ERROR_UNDERFLOW;

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

  _move_array_elements_lstart(dst, src, elements_to_move_amount);

  l->length = new_length;

  return CAL_SUCCESS;
}

/*******************************************************************************
 *    OVERFLOW UTILS
 ******************************************************************************/
#define _is_overflow_multi(a, b, max) (a != 0) && (b > max / a)
#define _is_overflow_add(a, b, max) (a > max - b)
#define _is_underflow_sub(a, b) (a < b)

bool _is_overflow_size_t_multi(size_t a, size_t b) {
  return _is_overflow_multi(a, b, CAL_SIZE_T_MAX);
}

bool _is_overflow_size_t_add(size_t a, size_t b) {
  return _is_overflow_add(a, b, CAL_SIZE_T_MAX);
}

bool _is_underflow_size_t_sub(size_t a, size_t b) {
  return _is_underflow_sub(a, b);
}

/*******************************************************************************
 *    ARRAY UTILS
 ******************************************************************************/

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from the end, allowing specific arrays overlapping.
 *  When dest is after source behaviour is well defined. Otherwise if it's not.
 *  If arrays do not overlap than it doesn't matter.
 */
void _move_array_elements_rstart(CAL_VALUE_TYPE dest[], CAL_VALUE_TYPE src[],
                                 size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];

#ifdef DEBUG_MOVE_POINTERS
    // This may cause bugs on some setups but is helpuful in testing
    src[n] = 0;
#endif
  }
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 * Moving starts from the beginning, allowing specific arrays overlapping.
 * When dest is before source behaviour is well defined. Otherwise if it's
 * not. If arrays do not overlap than it doesn't matter.
 */
void _move_array_elements_lstart(CAL_VALUE_TYPE dest[], CAL_VALUE_TYPE src[],
                                 size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[i] = src[i];

#ifdef DEBUG_MOVE_POINTERS
    // This may cause bugs on some setups but is helpuful in testing
    src[i] = 0;
#endif
  }
}
