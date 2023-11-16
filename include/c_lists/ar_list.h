/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

// App
#include "cll_error.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
typedef struct ar_list *arl_ptr;

// List operations
arl_ptr arl_create(size_t default_size);
void arl_destroy(arl_ptr l);
size_t arl_length(arl_ptr l);

// List's data operations
//// Getters
void *arl_get(arl_ptr l, size_t i);
void *arl_slice(arl_ptr l, size_t start_i, size_t elements_amount,
                void *slice[]);
//// Setters
arl_ptr arl_set(arl_ptr l, size_t i, void *value);
arl_ptr arl_append(arl_ptr l, void *value);
arl_ptr arl_insert(arl_ptr l, size_t i, void *value);
arl_ptr arl_insert_multi(arl_ptr l, size_t i, size_t v_len,
                         void *values[v_len]);
//// Removers
void *arl_pop(arl_ptr l, size_t i);
void *arl_pop_multi(arl_ptr l, size_t i, size_t elements_amount,
                    void *holder[]);
arl_ptr arl_remove(arl_ptr l, size_t i, void (*callback)(void *));
arl_ptr arl_clear(arl_ptr l, void (*callback)(void *));

#endif
