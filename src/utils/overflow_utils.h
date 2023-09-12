#ifndef _overflow_utils_h
#define _overflow_utils_h

#include "../l_def.h"

#include <stdbool.h>
#include <stddef.h>

bool is_overflow_int_multi(int a, int b);
bool is_overflow_size_t_multi(size_t a, size_t b);
bool is_overflow_size_l_multi(size_l a, size_l b);

#endif
