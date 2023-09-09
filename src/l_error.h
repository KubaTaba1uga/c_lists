#ifndef _l_error_h
#define _l_error_h

#include <assert.h>
#include <stddef.h>

typedef enum {
  L_SUCCESS = 0,

  L_ERROR_INVALID_ARGS,

  L_ERROR_OVERFLOW,

  L_ERROR_MEMORY_SHORTAGE,

  L_ERROR_INDEX_TOO_BIG,

  /* Enum assigns values automatically by incrementing */
  /*   the first value. */
  L_ERROR_LEN,
} l_error_t;

const char *const L_ERROR_STRINGS[] = {
    "Success",

    "Invalid arguments",

    "Overflow",

    "Not enough memory",
};

const char *l_strerror(l_error_t error);

#endif
