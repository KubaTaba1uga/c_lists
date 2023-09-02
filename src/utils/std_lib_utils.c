#include "std_lib_utils.h"

unsigned long get_current_time(void) {
  // Return number of seconds since Epoch.
  // On error returns 0.

  time_t now = time(NULL);
  if (now == (time_t)(-1))
    return 0;

  return (unsigned long)now;
}

void *app_malloc(size_t size) {
  // Interface to malloc
  return malloc(size);
}

int app_exit(int exit_code) {
  // Interface to exit
  exit(exit_code);
}
