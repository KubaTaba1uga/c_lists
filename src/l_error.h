#ifndef _l_error_h
#define _l_error_h

#include <stddef.h>

typedef enum {
  L_SUCCESS = 0,

  L_ERROR_INVALID_ARGS,

  L_ERROR_OVERFLOW,

  L_ERROR_OUT_OF_MEMORY,

  L_ERROR_INDEX_TOO_BIG,

  /* Enum assigns values automatically by incrementing
   *   the first value. `L_ERROR_LEN` stands for number
   *   of elements in enum (aka `length`).
   */
  L_ERROR_LEN,
} l_error_t;

const char *l_strerror(l_error_t error);

#endif
