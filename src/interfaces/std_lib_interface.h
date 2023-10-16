#ifndef _std_lib_interface_h
#define _std_lib_interface_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C Standard library
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void *app_malloc(size_t size);
void *app_realloc(void *p, size_t size);
void app_free(void *p) { free(p); }

#endif
