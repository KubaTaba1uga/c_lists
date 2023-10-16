#include "std_lib_interface.h"
#include <stdlib.h>

/* Just interface to malloc. */
void *app_malloc(size_t size) { return malloc(size); }

/* Just interface to realloc. */
void *app_realloc(void *p, size_t size) { return realloc(p, size); }

void app_free(void *p) { free(p); }
