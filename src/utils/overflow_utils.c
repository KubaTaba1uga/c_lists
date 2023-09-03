#include <limits.h>
#include <stdbool.h>

bool is_overflow_int_multi(int a, int b) {
  /* Detect overflow in integers multiplication */

  return (a != 0) && (b > INT_MAX / a);
}
