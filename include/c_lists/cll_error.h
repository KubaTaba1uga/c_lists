#ifndef _cll_error_h
#define _cll_error_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef enum {
  CLL_SUCCESS = 0,

  CLL_ERROR_INVALID_ARGS,

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
  CLL_ERROR_OVERFLOW,
  CLL_ERROR_UNDERFLOW,

  CLL_ERROR_OUT_OF_MEMORY,

  CLL_ERROR_INDEX_TOO_BIG,

  CLL_ERROR_REACHED_CAPACITY_MAX,

  CLL_ERROR_POP_EMPTY_LIST,
  /* Enum assigns values automatically by incrementing
   *   the first value. `CLL_ERROR_LEN` stands for number
   *   of elements in enum (aka `length`).
   */
  CLL_ERROR_LEN,
} cll_error;

const char *cll_strerror(cll_error error);

#endif
