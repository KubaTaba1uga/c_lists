#include <stddef.h>

// TO-DO
//  test moving overlapping arrays
//     1. Src one elem before dest
//     2. Dest one elem before src
//     3. Src n elem before dest
//     4. Dest n elem before src
//     5. Parametrize 3. and 4.

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from end to allow for some arrays overlapping.
 *  For example in algorithm for `move_indexes_by_positive_number`
 *     where pointers are moved within one array.
 *  In overlapping situation if dest is before src behaviour is undefined.
 */
void move_pointers_array_right(void *dest[], void *src[], size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];
    src[n] = NULL;
  }
}

void move_pointers_array_left(void *dest[], void *src[], size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[n] = src[n];
  }
}
