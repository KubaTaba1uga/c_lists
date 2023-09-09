#ifndef _l_error_h
#define _l_error_h

#include <assert.h>
#include <stddef.h>

typedef enum {
  L_SUCCESS = 0,

  L_ERROR_INVALID_ARGS,

  /* Enum assigns values automatically by incrementing */
  /*   the first value. */
  L_ERROR_LEN,
} l_error_t;

const char *const L_ERROR_STRINGS[] = {
    "SUCCESS",
};

// errors validation
/* int l_eqrror_strings_len = sizeof(L_ERROR_STRINGS) / sizeof(char *); */
/* assert(0); //(l_error_strings_len == L_ERROR_LEN)); */

const char *l_strerror(l_error_t error);

#endif
