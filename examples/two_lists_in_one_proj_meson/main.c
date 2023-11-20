#include "chr_list.h"
#include "int_list.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char c, char_data[] = "abcbcbcbcb";
  int int_data[] = {1, 2, 3, 4, 5};

  chr_ptr cl;
  int_ptr il;

  size_t i, len;
  int err;

  err = chr_create(&cl, 100);
  if (err) {
    puts(chr_strerror(err));
    return -1;
  }

  err = int_create(&il, 100);
  if (err) {
    puts(int_strerror(err));
    return -2;
  }

  for (i = 0; i < strlen(char_data); i++) {
    err = chr_append(cl, char_data[i]);
    if (err) {
      puts(chr_strerror(err));
      return -3;
    }
  }

  for (i = 0; i < sizeof(int_data) / sizeof(int); i++) {
    err = int_append(il, &int_data[i]);
    if (err) {
      puts(int_strerror(err));
      return -4;
    }
  }

  i = 0;
  do {
    err = chr_get(cl, i, &c);
    if (err) {
      puts(int_strerror(err));
      return -5;
    }

    printf("%c", c);

    chr_length(cl, &len);
  } while (++i < len);

  printf("\nSuccess\n");

  // Cleanup
  chr_destroy(cl);
  int_destroy(il);

  return 0;
}
