/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
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
    "Not enough memory",

    // 4
    "Index too big",
};

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
const char *l_strerror(l_error_t error) {
  // Return string on success, NULL on failure.

  if ((error >= L_ERROR_LEN) || (error < 0))
    return NULL;

  return L_ERROR_STRINGS[error];
}
