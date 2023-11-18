/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _cal_list_h
#define _cal_list_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

// App
#include "cll_def.h"
#include "cll_error.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef struct cal_def *cal_ptr;

// List operations
cll_error cal_create(cal_ptr *l, size_t default_size);
cll_error cal_destroy(cal_ptr l);
cll_error cal_length(cal_ptr l, size_t *length);

// List's data operations
//// Getters
cll_error cal_get(cal_ptr l, size_t i, CLL_VALUE_TYPE *value);
cll_error cal_slice(cal_ptr l, size_t start_i, size_t elements_amount,
                    CLL_VALUE_TYPE slice[]);
//// Setters
cll_error cal_set(cal_ptr l, size_t i, CLL_VALUE_TYPE value);
cll_error cal_insert(cal_ptr l, size_t i, CLL_VALUE_TYPE value);
cll_error cal_append(cal_ptr l, CLL_VALUE_TYPE value);
cll_error cal_insert_multi(cal_ptr l, size_t i, size_t v_len,
                           CLL_VALUE_TYPE values[v_len]);
//// Removers
cll_error cal_pop(cal_ptr l, size_t i, CLL_VALUE_TYPE *value);
cll_error cal_pop_multi(cal_ptr l, size_t i, size_t elements_amount,
                        CLL_VALUE_TYPE holder[]);
cll_error cal_remove(cal_ptr l, size_t i, void (*callback)(CLL_VALUE_TYPE));
cll_error cal_clear(cal_ptr l, void (*callback)(CLL_VALUE_TYPE));

#endif
