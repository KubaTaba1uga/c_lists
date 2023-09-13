/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h
/* #include "l_def.h" */
#include "l_error.h"

typedef struct {
  /* Number of elements.*/
  size_t length;

  /* Maximum number of elements. */
  size_t capacity;

  /* Storage. */
  void **array;
} ar_list;

l_error_t arl_init(ar_list *l, size_t default_size);
l_error_t arl_get(ar_list *l, size_t i, void **p);
l_error_t arl_set(ar_list *l, size_t i, void *value);

#endif
