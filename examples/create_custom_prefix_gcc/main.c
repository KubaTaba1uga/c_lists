#include "cal_list.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char string[] = "abcbcbcbcb";
  cal_ptr l;
  size_t i;
  cal_error err;

  err = cal_create(&l, 100);
  if (err) {
    puts(cal_strerror(err));
    return -1;
  }

  for (i = 0; i < strlen(string); i++) {
    err = cal_append(l, &string[i]);
    if (err) {
      puts(cal_strerror(err));
      return -2;
    }
  }

  printf("Success\n");

  // Cleanup
  cal_destroy(l);

  return 0;
}
