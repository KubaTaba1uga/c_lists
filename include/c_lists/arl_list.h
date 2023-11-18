/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _arl_list_h
#define _arl_list_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

// App
#include "cll_def.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef enum {
  ARL_SUCCESS = 0,

  ARL_ERROR_INVALID_ARGS,

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
  ARL_ERROR_OVERFLOW,
  ARL_ERROR_UNDERFLOW,

  ARL_ERROR_OUT_OF_MEMORY,

  ARL_ERROR_INDEX_TOO_BIG,

  ARL_ERROR_REACHED_CAPACITY_MAX,

  ARL_ERROR_POP_EMPTY_LIST,
  /* Enum assigns values automatically by incrementing
   *   the first value. `ARL_ERROR_LEN` stands for number
   *   of elements in enum (aka `length`).
   */
  ARL_ERROR_LEN,
} arl_error;

typedef struct arl_def *arl_ptr;

// List operations
arl_error arl_create(arl_ptr *l, size_t default_size);
arl_error arl_destroy(arl_ptr l);
arl_error arl_length(arl_ptr l, size_t *length);
const char *arl_strerror(arl_error error);

// List's data operations
//// Getters
arl_error arl_get(arl_ptr l, size_t i, CLL_VALUE_TYPE *value);
arl_error arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                    CLL_VALUE_TYPE slice[]);
//// Setters
arl_error arl_set(arl_ptr l, size_t i, CLL_VALUE_TYPE value);
arl_error arl_insert(arl_ptr l, size_t i, CLL_VALUE_TYPE value);
arl_error arl_append(arl_ptr l, CLL_VALUE_TYPE value);
arl_error arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                           CLL_VALUE_TYPE values[v_len]);
//// Removers
arl_error arl_pop(arl_ptr l, size_t i, CLL_VALUE_TYPE *value);
arl_error arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                        CLL_VALUE_TYPE holder[]);
arl_error arl_remove(arl_ptr l, size_t i, void (*callback)(CLL_VALUE_TYPE));
arl_error arl_clear(arl_ptr l, void (*callback)(CLL_VALUE_TYPE));

#endif
