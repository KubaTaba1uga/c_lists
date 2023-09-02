#include "std_lib_utils.h"

void *app_malloc(size_t size) {
  // Interface is required to mock it.
  return malloc(size);
}

size_t get_pointer_size(void) { return sizeof(void *); }
