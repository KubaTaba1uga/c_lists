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
#include "cll_error.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef struct arl_def *arl_ptr;

// List operations
cll_error arl_create(arl_ptr *l, size_t default_size);
cll_error arl_destroy(arl_ptr l);
cll_error arl_length(arl_ptr l, size_t *length);

// List's data operations
//// Getters
cll_error arl_get(arl_ptr l, size_t i, CLL_VALUE_TYPE *value);
cll_error arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                    CLL_VALUE_TYPE slice[]);
//// Setters
cll_error arl_set(arl_ptr l, size_t i, CLL_VALUE_TYPE value);
cll_error arl_insert(arl_ptr l, size_t i, CLL_VALUE_TYPE value);
cll_error arl_append(arl_ptr l, CLL_VALUE_TYPE value);
cll_error arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                           CLL_VALUE_TYPE values[v_len]);
//// Removers
cll_error arl_pop(arl_ptr l, size_t i, CLL_VALUE_TYPE *value);
cll_error arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                        CLL_VALUE_TYPE holder[]);
cll_error arl_remove(arl_ptr l, size_t i, void (*callback)(CLL_VALUE_TYPE));
cll_error arl_clear(arl_ptr l, void (*callback)(CLL_VALUE_TYPE));

#endif