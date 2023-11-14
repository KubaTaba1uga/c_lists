#ifndef _pointers_utils_h
#define _pointers_utils_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C Standard library
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void cll_move_pointers_array_rstart(void *dest[], void *src[], size_t n);

void cll_move_pointers_array_lstart(void *dest[], void *src[], size_t n);
#endif