/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h
#include "l_error.h"

typedef struct {

  /* The number of elements.*/
  size_t size;

  /* The size of the underlying array, */
  /* which is the maximum possible size without relocating data. */
  size_t capacity;

  void **array;
} ar_list;

l_error_t arl_init(ar_list *l, size_t default_size);
l_error_t arl_get(ar_list *l, size_t i, void **p);
l_error_t arl_set(ar_list *l, size_t i, void *value);

#endif
