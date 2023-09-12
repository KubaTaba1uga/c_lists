#ifndef _l_limits_h
#define _l_limits_h

#include "l_def.h"
#include <limits.h>

#define L_PTR_SIZE sizeof(void *);
#define L_SIZE_T_MAX (size_t) - 1;
#define L_CAPACITY_MAX L_SIZE_T_MAX / L_PTR_SIZE;
#define L_SIZE_L_MAX L_CAPACITY_MAX;

#endif
