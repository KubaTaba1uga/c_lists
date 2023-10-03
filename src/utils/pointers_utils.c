#include <stddef.h>
#include <stdio.h>
/* Moving is optimalized for performence. Please do not change moving
 * alghoritms, */
/* if new solution's time complexity is not better than O(n) */

// TO-DO
//  test moving overlapping arrays
//     1. Src one elem before dest
//     2. Dest one elem before src
//     3. Src n elem before dest
//     4. Dest n elem before src
//     5. Parametrize 3. and 4.

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from the end, allowing specific arrays overlapping.
 *  When dest is after source behaviour is well defined. Otherwise if it's not.
 *  If arrays do not overlap than it doesn't matter.
 */
void move_pointers_array_rstart(void *dest[], void *src[], size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];
    src[n] = NULL;
  }
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 * Moving starts from the beginning, allowing specific arrays overlapping.
 * When dest is before source behaviour is well defined. Otherwise if it's not.
 * If arrays do not overlap than it doesn't matter.
 */
void move_pointers_array_lstart(void *dest[], void *src[], size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[i] = src[i];
    src[i] = NULL;
  }
}
