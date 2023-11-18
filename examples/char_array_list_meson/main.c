#include "c_lists/arl_list.h"
#include "c_lists/cll_error.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char c, string[] = "I love penguins";
  arl_ptr l;
  size_t i, len;
  cll_error err;

  err = arl_create(&l, 100);
  if (err) {
    puts(cll_strerror(err));
    return -1;
  }

  for (i = 0; i < strlen(string); i++) {
    err = arl_append(l, string[i]);
    if (err) {
      puts(cll_strerror(err));
      return -2;
    }
  }

  i = 0;
  do {
    err = arl_get(l, i, &c);
    if (err) {
      puts(cll_strerror(err));
      return -3;
    }

    printf("%c", c);

    arl_length(l, &len);
  } while (++i < len);

  printf("\nSuccess\n");

  // Cleanup
  arl_destroy(l);

  return 0;
}
