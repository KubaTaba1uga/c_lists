/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stdlib.h>

// App
#include "cll_std_lib_interface.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void *app_malloc(size_t size) { return malloc(size); }

void *app_realloc(void *p, size_t size) { return realloc(p, size); }

void app_free(void *p) { free(p); }
