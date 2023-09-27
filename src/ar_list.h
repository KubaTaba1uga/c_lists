/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h
/* #include "l_def.h" */
#include "l_error.h"
#include <stddef.h>

typedef struct ar_list *arl_ptr;

l_error_t arl_create(arl_ptr *l_p, size_t default_size);
void arl_destroy(arl_ptr l);

l_error_t arl_get(arl_ptr l, size_t i, void **p);
l_error_t arl_set(arl_ptr l, size_t i, void *value);
l_error_t arl_append(arl_ptr l, void *value);
l_error_t arl_insert(arl_ptr l, size_t i, void *value);
#endif
