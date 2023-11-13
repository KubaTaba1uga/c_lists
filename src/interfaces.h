#ifndef _interfaces_src_protector_h
#define _interfaces_src_protector_h

#ifdef ENABLE_TESTS
#include "interfaces/std_lib_interface.h"

// Make local copies
#define local_malloc malloc
#define local_free free
#define local_realloc realloc

// Distinguish app's malloc from others
#define malloc app_malloc
#define realloc app_realloc
#define free app_free

#endif

#endif
