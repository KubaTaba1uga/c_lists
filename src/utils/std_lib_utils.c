#include "std_lib_utils.h"
#include <stdlib.h>

/* Just interface to malloc. */
void *app_malloc(size_t size) { return malloc(size); }

/* Just interface to realloc. */
void *app_realloc(void *p, size_t size) { return realloc(p, size); }

// TO-DO: ssubstitute this function with L_PTR_SIZE macro.
size_t get_pointer_size(void) { return sizeof(void *); }
