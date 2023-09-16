/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

/* Array list require allocating one continous chunk of memory. As all
 * allocating functions take required memory size in size_t type, it is safe to
 * assume that SIZE_MAX is maximum allocable memory size for one object.
 * To not shrink list accadidentially at some point ARL_MAX_CAPACITY
 * is defined as upper boundary. If You require bigger capacity check out other
 * lists.
 */

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
#include "l_def.h"
#include "l_error.h"
#include "l_limits.h"
#include "utils/overflow_utils.h"
#include "utils/std_lib_utils.h"

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
/* static l_error_t arl_grow_array_capacity(ar_list *l); */
/* static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
 */
/* size_t move_by); */

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* TO-DO */
/* 1. set up errno on failure */

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

  *p = l->array[i];

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

  l->array[i] = value;

  return L_SUCCESS;
}

/* void arl_append(ar_list *l, void *value) { */
/*   /\* Insert value to the last possible index. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */

/*   /\* size_t last_i = l->size; *\/ */

/*   /\* if (arl_is_i_too_big(l, last_i)) *\/ */
/*   arl_grow_array_capacity(l); */
/* } */

/* l_error_t arl_insert(ar_list *l, size_t i, void *value) { */
/*   /\* Insert value to the index. *\/ */
/*   /\* Return NULL on failure. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */
/*   void *p; */
/*   l_error_t err; */
/*   bool i_is_too_big; */

/*   arl_is_i_too_big(l, i, &i_is_too_big); */

/*   if (i_is_too_big) { */
/*     return L_ERROR_INDEX_TOO_BIG; */
/*   } */

/*   p = arl_move_indexes_by_positive_number(l, i, 1); */
/*   if (!p) */
/*     return L_ERROR_MEMORY_SHORTAGE; */

/*   err = arl_set(l, i, value); */
/*   if (err) */
/*     return err; */

/*   return L_SUCCESS; */
/* } */

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/

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

/* Move elements further by `move_by`, starting from `start_i`.
 * Set NULL on source value.
 * Ex:
 *    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2
 *    OUTPUT l.array {0, NULL, NULL, 1, 2}
 */
l_error_t arl_move_indexes_by_positive_number(arl_ptr l, size_t start_i,
                                              size_t move_by) {

  size_t old_length, new_length, i_source, i_dest, elements_to_move_amount;
  void *p;

  // Idea is to detect all failures upfront so recovery from half moved array
  //  is not required.
  if (is_overflow_size_t_add(l->length, move_by))
    return L_ERROR_OVERFLOW;

  old_length = l->length, new_length = l->length + move_by;

  if (new_length > (l->capacity) || new_length < (l->length))
    return L_ERROR_INVALID_ARGS;

  if (is_underflow_size_t_sub(old_length, start_i))
    return L_ERROR_OVERFLOW;

  elements_to_move_amount = old_length - start_i;

  if (elements_to_move_amount == 0)
    return L_SUCCESS;

  l->length = new_length, start_i--;

  for (; elements_to_move_amount > 0; elements_to_move_amount--) {
    i_source = start_i + elements_to_move_amount;
    i_dest = i_source + move_by;

    arl_get(l, i_source, &p);
    arl_set(l, i_dest, p);
    arl_set(l, i_source, NULL);
  }

  return L_SUCCESS;
}
