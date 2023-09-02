#ifndef _std_lib_utils_h
#define _std_lib_utils_h

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
// Standard library
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
unsigned long get_current_time(void);
void *app_malloc(size_t size);
int app_exit(int exit_code);

#endif
