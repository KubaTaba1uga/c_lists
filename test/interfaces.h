#ifndef _interfaces_test_protector_h
#define _interfaces_test_protector_h

#ifdef ENABLE_TESTS

// Clear app's assignments
#undef malloc
#undef realloc
#undef free

// Assign orginals
#define malloc local_malloc
#define realloc local_realloc
#define free local_free

#endif

#endif
