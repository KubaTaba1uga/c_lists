/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/* Array list require allocating one continous chunk of memory. As all
 * allocating functions take required memory size in size_t type, it is safe to
 * assume that SIZE_MAX is maximum allocable memory size for a list's instance.
 * List's array is not being shrinked, even on pop or clear. As it may be
 * usefull for someone, it is not essential to list's logic (in opposition
 * to growing). I would rather do some versions of current functions which
 * would shrink internall array, than edit current ones. Maybe even new list?
 */

/* Notes:
 * - Because of generic nature of the library, functions need to return
 *     the error. Example scenario:
 *         user want to use int as type
 *         user insert new value to list
 *         underneath sth failed
 *         what should we return to user to indicate the error?
 *      However this convetion is hard to use in other scenarios,
 *         getting length is a good example. If we always return error,
 *         we cannot do sth like:
 *            for (i=0; i < arl_length(l); i++)
 *              ....
 *         So as compromise, lib is trying to use returning by error as
 *         best effort principle. There might be exceptions of this rule
 *         each need to have use case described in function doc.
 *
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
#include "arl_list.h"
#ifdef ENABLE_TESTS
#include "cll_interfaces.h"
#endif

/*******************************************************************************
 *    PRIVATE DECLARATIONS
 ******************************************************************************/
struct arl_def {
  /* Number of elements.*/
  size_t length;

  /* Maximum number of elements. */
  size_t capacity;

  /* Storage. */
  ARL_VALUE_TYPE *array;
};

static bool _is_i_too_big(arl_ptr l, size_t i);
static void _get(arl_ptr l, size_t i, ARL_VALUE_TYPE *value);
static void _set(arl_ptr l, size_t i, ARL_VALUE_TYPE value);
static arl_error _grow_array_capacity(arl_ptr l);
static arl_error _move_elements_right(arl_ptr l, size_t start_i,
                                      size_t move_by);
static arl_error _move_elements_left(arl_ptr l, size_t start_i, size_t move_by);
// Pointers utils
static bool _is_overflow_size_t_multi(size_t a, size_t b);
static bool _is_overflow_size_t_add(size_t a, size_t b);
static bool _is_underflow_size_t_sub(size_t a, size_t b);
// Array utils
static void _move_array_elements_rstart(ARL_VALUE_TYPE dest[],
                                        ARL_VALUE_TYPE src[], size_t n);
static void _move_array_elements_lstart(ARL_VALUE_TYPE dest[],
                                        ARL_VALUE_TYPE src[], size_t n);
// Error utils
static const char *const ARL_ERROR_STRINGS[] = {
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
    "Popping empty list is disallowed",

};

static const size_t ARL_ERROR_STRINGS_LEN =
    sizeof(ARL_ERROR_STRINGS) / sizeof(char *);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* Creates array list's instance.
 * Behaviour is undefined if `default_capacity` is equal 0.
 */
arl_error arl_create(arl_ptr *l, size_t default_capacity) {
  if (_is_overflow_size_t_multi(default_capacity, ARL_VALUE_SIZE))
    return ARL_ERROR_OVERFLOW;

  void *l_array = malloc(default_capacity * ARL_VALUE_SIZE);

  if (!l_array)
    goto ERROR_OOM;

  arl_ptr l_local = malloc(sizeof(struct arl_def));

  if (!l_local)
    goto CLEANUP_L_LOCAL_OOM;

  l_local->array = (ARL_VALUE_TYPE *)l_array;
  l_local->capacity = default_capacity;
  l_local->length = 0;

  *l = l_local;

  return ARL_SUCCESS;

CLEANUP_L_LOCAL_OOM:
  free(l_array);
ERROR_OOM:
  return ARL_ERROR_OUT_OF_MEMORY;
}

/* Frees resouces allocated for array list's instance.
 */
arl_error arl_destroy(arl_ptr l) {
  free(l->array);
  free(l);

  return ARL_SUCCESS;
}

/* Returns list's length.
 *
 * !!!WARNING!!!
 * THIS FUNCTION DO NOT RETURN ERROR!!!
 * !!!WARNING!!!
 *
 * Length is often used in for loop.
 * Lib wants to be user friendly so we want to support sth like:
 *   for (i=0; i<srl_length(l); i++)
 *           ...
 */
size_t arl_length(arl_ptr l) { return l->length; }

/* Gets value under the index.
 */
arl_error arl_get(arl_ptr l, size_t i, ARL_VALUE_TYPE *value) {
  if (_is_i_too_big(l, i))
    return ARL_ERROR_INDEX_TOO_BIG;

  _get(l, i, value);

  return ARL_SUCCESS;
}

/* Fills slice with elements from index i till index
 *  i+elemets_amount. Start i and elements amount have
 *  to respect list's length, otherwise error is raised.
 *  Slice's length has to be bigger than elements amount,
 *  otherwise behaviour is undefined.
 */
arl_error arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                    ARL_VALUE_TYPE slice[]) {
  size_t k, last_elem_i;

  if (_is_i_too_big(l, start_i))
    return ARL_ERROR_INDEX_TOO_BIG;

  last_elem_i = start_i + elements_amount;
  if (_is_i_too_big(l, last_elem_i))
    return ARL_ERROR_INVALID_ARGS;

  for (k = 0; k < elements_amount; k++) {
    _get(l, k + start_i, &slice[k]);
  }

  return ARL_SUCCESS;
}

/* Sets value under the index.
 * Index has to be smaller than list's length.
 * Returns NULL and sets errno on failure.
 */
arl_error arl_set(arl_ptr l, size_t i, ARL_VALUE_TYPE value) {
  if (_is_i_too_big(l, i))
    return ARL_ERROR_INDEX_TOO_BIG;

  _set(l, i, value);

  return ARL_SUCCESS;
}

/* Insert one element under the index.
 * If index bigger than list's length, appends the value.
 */
arl_error arl_insert(arl_ptr l, size_t i, ARL_VALUE_TYPE value) {
  size_t new_length, move_by = 1;
  arl_error err;

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

  return ARL_SUCCESS;
}

/* Appends one element to the list's end.
 */
arl_error arl_append(arl_ptr l, ARL_VALUE_TYPE value) {
  return arl_insert(l, l->length + 1, value);
}

/* Insert multiple elements. Better optimized for multiple
 *  inserts than insert. Moving elements is done only once.
 *  i has to be smaller than l->length.
 *  values should hold valid pointers. v_len is values' length.
 */
arl_error arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                           ARL_VALUE_TYPE values[v_len]) {
  size_t new_length, k, move_by = v_len;
  arl_error err;

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

  return ARL_SUCCESS;
}

/* Pops element from under the index. Sets
 * value to the popped element's value.
 * If list is empty, returns ARL_ERROR_POP_EMPTY_LIST.
 * If i bigger tan list's lenth, substitues it with
 *  maximum poppable i.
 */
arl_error arl_pop(arl_ptr l, size_t i, ARL_VALUE_TYPE *value) {
  const size_t offset = 1;
  ARL_VALUE_TYPE value_holder;
  arl_error err;

  if (_is_i_too_big(l, i))
    i = l->length - 1;
  if (l->length == 0) {
    return ARL_ERROR_POP_EMPTY_LIST;
  }

  _get(l, i, &value_holder);

  err = _move_elements_left(l, ++i, offset);
  if (err)
    return err;

  *value = value_holder;

  return ARL_SUCCESS;
}

/* Fills holder with elements starting from index i till index
 *  i+elements aomunt. Holder's length has to be bigger than
 *  elements amount. Otherwise behaviour is undefined.
 *  Better optimized for multiple pops than pop. Elements are
 *  moved only once.
 */
arl_error arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                        ARL_VALUE_TYPE holder[]) {
  arl_error err;

  err = arl_slice(l, i, elements_amount, holder);
  if (err)
    return err;

  if (_is_overflow_size_t_add(i, elements_amount))
    return ARL_ERROR_OVERFLOW;

  err = _move_elements_left(l, i + elements_amount, elements_amount);
  if (err)
    return err;

  return ARL_SUCCESS;
}

/* Removes element from under the index.
 * Executes callback function on removed element,
 *  only if callback is not NULL.
 */
arl_error arl_remove(arl_ptr l, size_t i, void (*callback)(ARL_VALUE_TYPE)) {
  ARL_VALUE_TYPE p;
  arl_error err;

  err = arl_pop(l, i, &p);
  if (err)
    return err;

  if (callback)
    callback(p);

  return ARL_SUCCESS;
}

/* Removes all elements from the list.
 * Executes callback function on each removed element,
 *  only if callback is not NULL.
 */
arl_error arl_clear(arl_ptr l, void (*callback)(ARL_VALUE_TYPE)) {
  size_t i;
  ARL_VALUE_TYPE value;
  arl_error err;

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

  return ARL_SUCCESS;
}

/*******************************************************************************
 *    ERRORS UTILS
 ******************************************************************************/

const char *arl_strerror(arl_error error) {
  // Return string on success, NULL on failure.
  // This function is only exception from `always
  //  return arl_error_t` design decision.
  // Idea is to mimic org strerror.

  if ( // Upper bound
      (error >= ARL_ERROR_LEN) || (error >= ARL_ERROR_STRINGS_LEN) ||
      // Lower bound
      (error < 0))
    return NULL;

  return ARL_ERROR_STRINGS[error];
}

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
void _get(arl_ptr l, size_t i, ARL_VALUE_TYPE *value) { *value = l->array[i]; }
void _set(arl_ptr l, size_t i, ARL_VALUE_TYPE value) { l->array[i] = value; }

/* Checks if index is within list boundaries.
 * The behaviour is undefined if is not a valid pointer.
 */
bool _is_i_too_big(arl_ptr l, size_t i) { return i >= (l->length); }

/* Counts list's new capacity.
 */
static arl_error _count_new_capacity(size_t current_length,
                                     size_t current_capacity,
                                     size_t *new_capacity) {

  /* Size is always smaller than capacity. There is no need in checking
   * new_size divided by cur_size overflow.
   */
  if (_is_overflow_size_t_multi(current_length, 3) ||
      _is_overflow_size_t_add(current_capacity, 2)) {

    return ARL_ERROR_OVERFLOW;
  }

  *new_capacity = 3 * current_length / 2 + current_capacity;

  return ARL_SUCCESS;
}

/* Grows underlaying array. */
arl_error _grow_array_capacity(arl_ptr l) {
  void *p;
  size_t new_capacity;
  arl_error err;

  err = _count_new_capacity(l->length, l->capacity, &new_capacity);
  if (err)
    return err;

  p = realloc(l->array, new_capacity * ARL_VALUE_SIZE);
  if (!p) {
    return ARL_ERROR_OUT_OF_MEMORY;
  }

  l->capacity = new_capacity;
  l->array = p;

  return ARL_SUCCESS;
};

/* Move elements to the right by `move_by`, starting from `start_i`.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, , , 1, 2}
 */
arl_error _move_elements_right(arl_ptr l, size_t start_i, size_t move_by) {

  size_t new_length, elements_to_move_amount;

  // Idea is to detect all failures upfront so recovery from half moved
  // array
  //  is not required.
  if (_is_overflow_size_t_add(l->length, move_by))
    return ARL_ERROR_OVERFLOW;

  new_length = l->length + move_by;

  if (new_length > (l->capacity))
    return ARL_ERROR_INVALID_ARGS;

  if (_is_underflow_size_t_sub(l->length, start_i))
    return ARL_ERROR_UNDERFLOW;

  elements_to_move_amount = l->length - start_i;

  if (elements_to_move_amount == 0)
    return ARL_SUCCESS;

  _move_array_elements_rstart(l->array + start_i + move_by, l->array + start_i,
                              elements_to_move_amount);

  l->length = new_length;

  return ARL_SUCCESS;
}

/* Move elements to the left by `move_by`, starting from start_i
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 2, move_by 1
 *    OUTPUT l.array {1, 2, , ,}
 */
arl_error _move_elements_left(arl_ptr l, size_t start_i, size_t move_by) {
  /* ove_byop elements starting from index 5 till index 8
   *  If start_i is bigger or equal to l->length, assumes
   *   end of the list.
   */

  size_t new_length, elements_to_move_amount;
  ARL_VALUE_TYPE *src, *dst;

  // Do not allow reading before list's start.
  if (move_by > start_i)
    move_by = start_i;

  // Idea is to detect all failures upfront so recovery from half moved
  // array
  //  is not required.

  if (_is_underflow_size_t_sub(l->length, move_by))
    return ARL_ERROR_UNDERFLOW;

  new_length = l->length - move_by;

  if (_is_underflow_size_t_sub(l->length, start_i))
    return ARL_ERROR_UNDERFLOW;

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

  return ARL_SUCCESS;
}

/*******************************************************************************
 *    OVERFLOW UTILS
 ******************************************************************************/
#define _is_overflow_multi(a, b, max) (a != 0) && (b > max / a)
#define _is_overflow_add(a, b, max) (a > max - b)
#define _is_underflow_sub(a, b) (a < b)

bool _is_overflow_size_t_multi(size_t a, size_t b) {
  return _is_overflow_multi(a, b, ARL_SIZE_T_MAX);
}

bool _is_overflow_size_t_add(size_t a, size_t b) {
  return _is_overflow_add(a, b, ARL_SIZE_T_MAX);
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
void _move_array_elements_rstart(ARL_VALUE_TYPE dest[], ARL_VALUE_TYPE src[],
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
void _move_array_elements_lstart(ARL_VALUE_TYPE dest[], ARL_VALUE_TYPE src[],
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
