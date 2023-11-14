/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include "cll_error.h"

// App
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef struct ar_list *arl_ptr;

// List operations
cll_error_t arl_create(arl_ptr *l_p, size_t default_size);
void arl_destroy(arl_ptr l);
size_t arl_length(arl_ptr l);

// List's data operations
cll_error_t arl_get(arl_ptr l, size_t i, void **p);
cll_error_t arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                      void *slice[]);
cll_error_t arl_set(arl_ptr l, size_t i, void *value);
cll_error_t arl_insert(arl_ptr l, size_t i, void *value);
cll_error_t arl_append(arl_ptr l, void *value);
cll_error_t arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                             void *values[v_len]);
cll_error_t arl_pop(arl_ptr l, size_t i, void **value);
cll_error_t arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                          void *holder[]);
cll_error_t arl_remove(arl_ptr l, size_t i, void (*callback)(void *));
cll_error_t arl_clear(arl_ptr l, void (*callback)(void *));

#endif
