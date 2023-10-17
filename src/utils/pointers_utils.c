/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>
#include <stdio.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

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
 * When dest is before source behaviour is well defined. Otherwise if it's
 * not. If arrays do not overlap than it doesn't matter.
 */
void move_pointers_array_lstart(void *dest[], void *src[], size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[i] = src[i];
    src[i] = NULL;
  }
}
