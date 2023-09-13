/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include "l_error.h"
#include <stddef.h>

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
    "Not enough memory",

    // 4
    "Index too big",
};

static const size_t L_ERROR_STRINGS_LEN =
    sizeof(L_ERROR_STRINGS) / sizeof(char *);

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
// TO-DO
// return l_error_t
const char *l_strerror(l_error_t error) {
  // Return string on success, NULL on failure.

  if ( // Upper bound
      (error >= L_ERROR_LEN) || (error >= L_ERROR_STRINGS_LEN) ||
      // Lower bound
      (error < 0))
    return NULL;

  return L_ERROR_STRINGS[error];
}
