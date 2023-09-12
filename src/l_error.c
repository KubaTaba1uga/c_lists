#include "l_error.h"

const char *const L_ERROR_STRINGS[] = {
    "Success",

    "Invalid arguments",

    "Overflow",

    "Not enough memory",

    "Index too big",
};

const char *l_strerror(l_error_t error) {
  // Return string on success, NULL on failure.

  if ((error >= L_ERROR_LEN) || (error < 0))
    return NULL;

  return L_ERROR_STRINGS[error];
}
