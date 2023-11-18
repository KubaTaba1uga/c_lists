/* #include "chr_list.h" */
#include "int_list.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char chr_data[] = "abcbcbcbcb";
  int int_data[] = {1, 2, 3, 4, 5};

  /* chr_ptr cl; */
  /* int_ptr il; */

  /* size_t i; */
  /* int err; */

  /* err = chr_create(&cl, 100); */
  /* if (err) { */
  /*   puts(chr_strerror(err)); */
  /*   return -1; */
  /* } */

  /* err = int_create(&il, 100); */
  /* if (err) { */
  /*   puts(int_strerror(err)); */
  /*   return -2; */
  /* } */

  /* for (i = 0; i < strlen(chr_data); i++) { */
  /*   err = cal_append(l, &string[i]); */
  /*   if (err) { */
  /*     puts(cll_strerror(err)); */
  /*     return -2; */
  /*   } */
  /* } */

  printf("Success\n");

  // Cleanup
  /* cal_destroy(l); */

  return 0;
}
