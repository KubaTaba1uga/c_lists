/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

/*******************************************************************************
 *    PRIVATE API
 ******************************************************************************/
#define is_overflow_multi(a, b, max) (a != 0) && (b > max / a)

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
bool is_overflow_int_multi(int a, int b) {
  /* Detect overflow in integers multiplication */

  return is_overflow_multi(a, b, INT_MAX);
}

bool is_overflow_size_t_multi(size_t a, size_t b) {
  return is_overflow_multi(a, b, ULONG_MAX);
}
