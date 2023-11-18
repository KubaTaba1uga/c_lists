#include "c_lists/arl_list.h"
#include "c_lists/cal_list.h"
#include "c_lists/cll_error.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char string[] = "abcbcbcbcb";
  cal_ptr l;
  size_t i;
  cll_error err;

  err = cal_create(&l, 100);
  if (err) {
    puts(cll_strerror(err));
    return -1;
  }

  for (i = 0; i < strlen(string); i++) {
    err = cal_append(l, &string[i]);
    if (err) {
      puts(cll_strerror(err));
      return -2;
    }
  }

  printf("Success\n");

  // Cleanup
  cal_destroy(l);

  return 0;
}
