/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

// App
#include "c_lists/cll_error.h"

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/
static const char *const CLL_ERROR_STRINGS[] = {
    // 0
    "Success",

    // 1
    "Invalid arguments",

    // 2
    "Overflow",

    // 3
    "Underflow",

    // 4
    "Not enough memory",

    // 5
    "Index too big",

    // 6
    "Array's maximum capacity reached",

    // 7
    "Popping empty list is disallowed",

};

static const size_t CLL_ERROR_STRINGS_LEN =
    sizeof(CLL_ERROR_STRINGS) / sizeof(char *);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

const char *cll_strerror(cll_error error) {
  // Return string on success, NULL on failure.
  // This function is only exception from `always
  //  return cll_error_t` design decision.
  // Idea is to mimic org strerror.

  if ( // Upper bound
      (error >= CLL_ERROR_LEN) || (error >= CLL_ERROR_STRINGS_LEN) ||
      // Lower bound
      (error < 0))
    return NULL;

  return CLL_ERROR_STRINGS[error];
}
