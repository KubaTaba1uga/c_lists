/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "c_lists/cll_def.h"

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
#define _is_overflow_multi(a, b, max) (a != 0) && (b > max / a)
#define _is_overflow_add(a, b, max) (a > max - b)
#define _is_overflow_sub(a, b) (a < b)

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
bool cll_is_overflow_int_multi(int a, int b) {
  return _is_overflow_multi(a, b, INT_MAX);
}

bool cll_is_overflow_size_t_multi(size_t a, size_t b) {
  return _is_overflow_multi(a, b, CLL_SIZE_T_MAX);
}

bool cll_is_overflow_size_t_add(size_t a, size_t b) {
  return _is_overflow_add(a, b, CLL_SIZE_T_MAX);
}

bool cll_is_underflow_size_t_sub(size_t a, size_t b) {
  return _is_overflow_sub(a, b);
}
