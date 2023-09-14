/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "../l_def.h"

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
#define is_overflow_multi(a, b, max) (a != 0) && (b > max / a)
#define is_overflow_add(a, b, max) (a > max - b)
#define is_overflow_sub(a, b) (a > b)
/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
bool is_overflow_int_multi(int a, int b) {
  /* Detect overflow in integers multiplication */

  return is_overflow_multi(a, b, INT_MAX);
}

bool is_overflow_size_t_multi(size_t a, size_t b) {
  return is_overflow_multi(a, b, L_SIZE_T_MAX);
}

bool is_overflow_l_capacity_multi(size_t a, size_t b) {
  return is_overflow_multi(a, b, ARL_CAPACITY_MAX);
}

bool is_overflow_size_t_add(size_t a, size_t b) {
  return is_overflow_add(a, b, L_SIZE_T_MAX);
}

bool is_overflow_l_capacity_add(size_t a, size_t b) {
  return is_overflow_add(a, b, ARL_CAPACITY_MAX);
}

bool is_underflow_size_t_sub(size_t a, size_t b) {
  return is_overflow_sub(a, b);
}
