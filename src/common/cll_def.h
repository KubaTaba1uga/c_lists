#ifndef _cll_def_h
#define _cll_def_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
#define CLL_PTR_SIZE sizeof(void *)
#define CLL_SIZE_T_MAX (size_t) - 1
#define ARL_CAPACITY_MAX CLL_SIZE_T_MAX / CLL_PTR_SIZE

#endif