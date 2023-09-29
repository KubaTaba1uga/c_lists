#include <stddef.h>
#include <stdio.h>

// TO-DO
//  test moving overlapping arrays
//     1. Src one elem before dest
//     2. Dest one elem before src
//     3. Src n elem before dest
//     4. Dest n elem before src
//     5. Parametrize 3. and 4.

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from end allows for specific arrays overlapping.
 *  It is allowed when dest is after src in the same array.
 *  For example in algorithm for `move_indexes_by_positive_number`.
 *  In overlapping situation if dest is before src behaviour is undefined.
 */
void move_pointers_array_rstart(void *dest[], void *src[], size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];
    src[n] = NULL;
  }
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 * Moving starts from the beginning to allow for specific arrays overlapping.
 * When dest is before source behaviour is well defined. Otherwise if it's not.
 */
void move_pointers_array_lstart(void *dest[], void *src[], size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[i] = src[i];
    src[i] = NULL;
  }
}
