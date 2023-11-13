#ifndef _overflow_utils_h
#define _overflow_utils_h

/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
// C Standard library
#include <stdbool.h>
#include <stddef.h>

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
bool cll_is_overflow_int_multi(int a, int b);
bool cll_is_overflow_size_t_multi(size_t a, size_t b);
bool cll_is_overflow_l_capacity_multi(size_t a, size_t b);
bool cll_is_overflow_size_t_add(size_t a, size_t b);
bool cll_is_overflow_l_capacity_add(size_t a, size_t b);
bool cll_is_underflow_size_t_sub(size_t a, size_t b);
#endif
