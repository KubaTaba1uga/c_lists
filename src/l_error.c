/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

// App
#include "l_error.h"

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/
static const char *const L_ERROR_STRINGS[] = {
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

    // 6
    "Popping empty list is disallowed",

};

static const size_t L_ERROR_STRINGS_LEN =
    sizeof(L_ERROR_STRINGS) / sizeof(char *);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/

const char *l_strerror(l_error_t error) {
  // Return string on success, NULL on failure.
  // This function is only exception from `always
  //  return l_error_t` design decision.
  // Idea is to mimic org strerror.

  if ( // Upper bound
      (error >= L_ERROR_LEN) || (error >= L_ERROR_STRINGS_LEN) ||
      // Lower bound
      (error < 0))
    return NULL;

  return L_ERROR_STRINGS[error];
}
