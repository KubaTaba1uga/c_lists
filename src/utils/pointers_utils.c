#include "../l_error.h"
#include <stddef.h>
#include <stdio.h>

/* Copy pointers from `src` to `dest`. The
 *
 */
void pointers_move(void **dest, void **src, size_t pointers_amount) {
  while (pointers_amount-- > 0) {
    dest[pointers_amount] = src[pointers_amount];
  }
}
/* Copy pointers from `src` to `dest`. Copying is done
 *  starting from end, so arrays can overlap.
 */
void pointers_copy(size_t dl, void *dest[dl], size_t sl, void *src[sl]) {
  size_t limit = dl > sl ? sl : dl;

  while (limit-- > 0) {
    dest[limit] = src[limit];
  }
}
