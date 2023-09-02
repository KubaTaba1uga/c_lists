/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */

#ifndef _ar_list_h
#define _ar_list_h
#include "utils/std_lib_utils.h"

typedef struct {

  /* The number of elements.*/
  size_t size;

  /* The size of the underlying array, */
  /* which is the maximum possible size without relocating data. */
  size_t capacity;

  void **array;
} ar_list;

extern ar_list *arl_init(ar_list *l, size_t default_size);
extern void *arl_get(ar_list *l, size_t i);
extern void *arl_set(ar_list *l, size_t i, void *value);

#endif
