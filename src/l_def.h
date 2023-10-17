#ifndef _l_def_h
#define _l_def_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C standard library
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
#define L_PTR_SIZE sizeof(void *)
#define L_SIZE_T_MAX (size_t) - 1
#define ARL_CAPACITY_MAX L_SIZE_T_MAX / L_PTR_SIZE

#endif
