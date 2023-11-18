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
#define CLL_SIZE_T_MAX (size_t) - 1

// Value's type macro
#ifndef CLL_VALUE_TYPE
#define CLL_VALUE_TYPE void *
#endif

// Value's type's utils
#define CLL_VALUE_SIZE sizeof(CLL_VALUE_TYPE)

#endif
