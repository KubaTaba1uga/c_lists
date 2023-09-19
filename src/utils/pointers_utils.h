#include <stddef.h>

void pointers_move(void **dest, void **src, size_t pointers_amount);
void pointers_copy(size_t dl, void *dest[dl], size_t sl, void *src[sl]);
