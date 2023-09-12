#ifndef _l_def_h
#define _l_def_h

#include <stddef.h>

#define L_PTR_SIZE sizeof(void *);
#define L_SIZE_T_MAX (size_t) - 1;
#define L_CAPACITY_MAX L_SIZE_T_MAX / L_PTR_SIZE;
#define L_SIZE_L_MAX L_CAPACITY_MAX;

typedef size_t l_size_l;

#endif
