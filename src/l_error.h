#ifndef _l_error_h
#define _l_error_h

#include <stddef.h>

typedef enum {
  L_SUCCESS = 0,

  L_ERROR_INVALID_ARGS,

  /* Overflow usually is caused by invalid args,
   *  assuming it's relation to INVALID_ARGS error
   *  may be profitable during debugging. My goal
   *  in splitting those is giving You capability
   *  to handle those scenarios separatelly.
   *  Moving responsibility to You would require
   *  knowing each calculation made by the lib (by You).
   *  Just thinking about it gives me a hadache :P
   *
   * TO-DO Add more overflow handlings to lib itself.
   */
  L_ERROR_OVERFLOW,
  L_ERROR_UNDERFLOW,

  L_ERROR_OUT_OF_MEMORY,

  L_ERROR_INDEX_TOO_BIG,

  L_ERROR_REACHED_CAPACITY_MAX,

  L_ERROR_POP_EMPTY_LIST,
  /* Enum assigns values automatically by incrementing
   *   the first value. `L_ERROR_LEN` stands for number
   *   of elements in enum (aka `length`).
   */
  L_ERROR_LEN,
} l_error_t;

const char *l_strerror(l_error_t error);

#endif
