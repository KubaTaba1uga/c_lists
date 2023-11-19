/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _cal_list_h
#define _cal_list_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

/*******************************************************************************
 *    MACRO
 ******************************************************************************/
#define CAL_SIZE_T_MAX (size_t) - 1

// Value's type macro
#ifndef CAL_VALUE_TYPE
#define CAL_VALUE_TYPE void *
#endif

// Value's type's utils
#define CAL_VALUE_SIZE sizeof(CAL_VALUE_TYPE)

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef enum {
  CAL_SUCCESS = 0,

  CAL_ERROR_INVALID_ARGS,

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
  CAL_ERROR_OVERFLOW,
  CAL_ERROR_UNDERFLOW,

  CAL_ERROR_OUT_OF_MEMORY,

  CAL_ERROR_INDEX_TOO_BIG,

  CAL_ERROR_REACHED_CAPACITY_MAX,

  CAL_ERROR_POP_EMPTY_LIST,
  /* Enum assigns values automatically by incrementing
   *   the first value. `CAL_ERROR_LEN` stands for number
   *   of elements in enum (aka `length`).
   */
  CAL_ERROR_LEN,
} cal_error;

typedef struct cal_def *cal_ptr;

// List operations
cal_error cal_create(cal_ptr *l, size_t default_size);
cal_error cal_destroy(cal_ptr l);
cal_error cal_length(cal_ptr l, size_t *length);
const char *cal_strerror(cal_error error);

// List's data operations
//// Getters
cal_error cal_get(cal_ptr l, size_t i, CAL_VALUE_TYPE *value);
cal_error cal_slice(cal_ptr l, size_t start_i, size_t elements_amount,
                    CAL_VALUE_TYPE slice[]);
//// Setters
cal_error cal_set(cal_ptr l, size_t i, CAL_VALUE_TYPE value);
cal_error cal_insert(cal_ptr l, size_t i, CAL_VALUE_TYPE value);
cal_error cal_append(cal_ptr l, CAL_VALUE_TYPE value);
cal_error cal_insert_multi(cal_ptr l, size_t i, size_t v_len,
                           CAL_VALUE_TYPE values[v_len]);
//// Removers
cal_error cal_pop(cal_ptr l, size_t i, CAL_VALUE_TYPE *value);
cal_error cal_pop_multi(cal_ptr l, size_t i, size_t elements_amount,
                        CAL_VALUE_TYPE holder[]);
cal_error cal_remove(cal_ptr l, size_t i, void (*callback)(CAL_VALUE_TYPE));
cal_error cal_clear(cal_ptr l, void (*callback)(CAL_VALUE_TYPE));

#endif
