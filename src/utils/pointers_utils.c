#include "../l_error.h"
#include <stddef.h>

/* Move pointers from `dest` to `src`.
 * Memmove assumes that pointer content is unsigned char.
 */
void pointers_move(void **dest, void **src, size_t pointers_amount) {
  for (; pointers_amount > 0; pointers_amount--) {
    *(dest + pointers_amount) = *(src + pointers_amount);
  }
}
