#include "myprefix_list.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char myprefix_data[] = "abcbcbcbcb";

  myprefix_ptr l;

  size_t i;
  int err;

  err = myprefix_create(&l, 100);
  if (err) {
    puts(myprefix_strerror(err));
    return -1;
  }

  for (i = 0; i < strlen(myprefix_data); i++) {
    err = myprefix_append(l, &myprefix_data[i]);
    if (err) {
      puts(myprefix_strerror(err));
      return -3;
    }
  }

  printf("Success\n");

  // Cleanup
  /* cal_destroy(l); */

  return 0;
}
