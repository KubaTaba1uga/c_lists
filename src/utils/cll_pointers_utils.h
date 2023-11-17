#ifndef _pointers_utils_h
#define _pointers_utils_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C Standard library
#include <stddef.h>

#include "c_lists/cll_def.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void cll_move_pointers_array_rstart(CLL_VALUE_TYPE dest[], CLL_VALUE_TYPE src[],
                                    size_t n);

void cll_move_pointers_array_lstart(CLL_VALUE_TYPE dest[], CLL_VALUE_TYPE src[],
                                    size_t n);
#endif
