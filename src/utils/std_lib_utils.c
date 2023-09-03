#include "std_lib_utils.h"
#include <stdlib.h>

void *app_malloc(size_t size) { return malloc(size); }
void *app_realloc(void *p, size_t size) { return realloc(p, size); }

size_t get_pointer_size(void) { return sizeof(void *); }
