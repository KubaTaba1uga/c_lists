/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>
#include <stdio.h>

#include "c_lists/cll_def.h"
/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from the end, allowing specific arrays overlapping.
 *  When dest is after source behaviour is well defined. Otherwise if it's not.
 *  If arrays do not overlap than it doesn't matter.
 */
void cll_move_pointers_array_rstart(CLL_VALUE_TYPE dest[], CLL_VALUE_TYPE src[],
                                    size_t n) {
  while (n-- > 0) {
    dest[n] = src[n];

#ifdef DEBUG_MOVE_POINTERS
    // This may cause bugs on some setups but is helpuful in testing
    src[n] = NULL;
#endif
  }
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 * Moving starts from the beginning, allowing specific arrays overlapping.
 * When dest is before source behaviour is well defined. Otherwise if it's
 * not. If arrays do not overlap than it doesn't matter.
 */
void cll_move_pointers_array_lstart(CLL_VALUE_TYPE dest[], CLL_VALUE_TYPE src[],
                                    size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    dest[i] = src[i];

#ifdef DEBUG_MOVE_POINTERS
    // This may cause bugs on some setups but is helpuful in testing
    src[i] = NULL;
#endif
  }
}
