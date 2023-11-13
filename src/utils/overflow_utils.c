/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "../common/l_def.h"

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
#define _is_overflow_multi(a, b, max) (a != 0) && (b > max / a)
#define _is_overflow_add(a, b, max) (a > max - b)
#define _is_overflow_sub(a, b) (a < b)

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
bool l_is_overflow_int_multi(int a, int b) {
  /* Detect overflow in integers multiplication */

  return _is_overflow_multi(a, b, INT_MAX);
}

bool l_is_overflow_size_t_multi(size_t a, size_t b) {
  return _is_overflow_multi(a, b, L_SIZE_T_MAX);
}

bool l_is_overflow_l_capacity_multi(size_t a, size_t b) {
  return _is_overflow_multi(a, b, ARL_CAPACITY_MAX);
}

bool l_is_overflow_size_t_add(size_t a, size_t b) {
  return _is_overflow_add(a, b, L_SIZE_T_MAX);
}

bool l_is_overflow_l_capacity_add(size_t a, size_t b) {
  return _is_overflow_add(a, b, ARL_CAPACITY_MAX);
}

bool l_is_underflow_size_t_sub(size_t a, size_t b) {
  return _is_overflow_sub(a, b);
}
