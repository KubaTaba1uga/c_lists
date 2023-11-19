#include "char_list.h"
#include "examples/create_custom_prefix_meson/build/subprojects/c_lists_int/int_list.h"
#include "int_list.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char char_data[] = "abcbcbcbcb";
  int int_data[] = {1, 2, 3, 4, 5};

  char_ptr cl;
  int_ptr il;

  size_t i, len;
  int err;

  err = char_create(&cl, 100);
  if (err) {
    puts(char_strerror(err));
    return -1;
  }

  err = int_create(&il, 100);
  if (err) {
    puts(int_strerror(err));
    return -2;
  }

  for (i = 0; i < strlen(char_data); i++) {
    err = char_append(cl, char_data[i]);
    if (err) {
      puts(char_strerror(err));
      return -3;
    }
  }

  for (i = 0; i < sizeof(int_data) / sizeof(int); i++) {
    err = int_append(il, int_data[i]);
    if (err) {
      puts(int_strerror(err));
      return -4;
    }
  }

  do {
    char_length(il, &len);
  } while (i = 0; i < len; i++) {
  }

  printf("Success\n");

  // Cleanup
  char_destroy(cl);
  int_destroy(ci);

  return 0;
}
