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

// TO-DO
//  test moving overlapping arrays
//     1. Src one elem before dest
//     2. Dest one elem before src
//     3. Src n elem before dest
//     4. Dest n elem before src
//     5. Parametrize 3. and 4.

/* Copy pointers from `src` to `dest`.
 *  Copying is done from end to allow for some arrays overlapping.
 *  For example in algorithm for `move_indexes_by_positive_number`
 *  where pointers are moved within one array.
 *  Overlapping is shown in `move_pointers_array`'s tests.
 */
void move_pointers_array(void *dest[], void *src[], size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];
  }
}
