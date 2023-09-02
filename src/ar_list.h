/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stddef.h>

typedef struct {

  /* The number of elements.*/
  size_t size;

  /* The size of the underlying array, */
  /* which is the maximum possible size without relocating data. */
  size_t capacity;

  void **array;
} ar_list;

extern void *arl_init(ar_list *l, size_t default_size);
extern void *arl_get(ar_list *l, size_t i);
extern void *arl_set(ar_list *l, size_t i, void *value);

static void *arl_alloc_array(ar_list *l, size_t default_size);
static size_t arl_count_new_capacity(size_t size, size_t capacity);
static size_t arl_count_new_capacity_base(size_t size, size_t capacity);
static bool arl_is_i_invalid(ar_list *l, size_t i);
static void *arl_grow_array_capacity(ar_list *l);
static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
                                                 size_t move_by);
