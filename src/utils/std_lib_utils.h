#ifndef _std_lib_utils_h
#define _std_lib_utils_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C Standard library
#include <stdlib.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void *app_malloc(size_t size);
void *app_realloc(void *p, size_t size);

size_t get_pointer_size(void);

#endif
