#include <stddef.h>
#include <stdio.h>
/* Moving is optimalized for performence. Please do not change moving
 * alghoritms, */
/* if new solution's time complexity is not better than O(n) */

// TO-DO
//  test moving overlapping arrays
//     1. Src one elem before dest
//     2. Dest one elem before src
//     3. Src n elem before dest
//     4. Dest n elem before src
//     5. Parametrize 3. and 4.

void print_src_dest(void *p) {
  if (!p)
    printf("NULL");
  else
    printf("%i", *(int *)p);
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 *  Moving starts from the end, allowing specific arrays overlapping.
 *  When dest is after source behaviour is well defined. Otherwise if it's not.
 *  If arrays do not overlap than it doesn't matter.
 */
void move_pointers_array_rstart(void *dest[], void *src[], size_t n) {
  /* printf("START: n=%lu\n", n); */
  while (n-- > 0) {
    /* printf("Before: dest[%lu]=", n); */
    /* print_src_dest(dest[n]); */
    /* printf(" src[%lu]=", n); */
    /* print_src_dest(src[n]); */
    /* puts(""); */

    dest[n] = src[n];
    src[n] = NULL;

    /* printf("After: dest[%lu]=", n); */
    /* print_src_dest(dest[n]); */
    /* printf(" src[%lu]=", n); */
    /* print_src_dest(src[n]); */
    /* puts(""); */
  }
}

/* Move pointers from `src` to `dest`. N is number of elements to move.
 * Moving starts from the beginning, allowing specific arrays overlapping.
 * When dest is before source behaviour is well defined. Otherwise if it's
 * not. If arrays do not overlap than it doesn't matter.
 */
void move_pointers_array_lstart(void *dest[], void *src[], size_t n) {
  size_t i;

  printf("START: n=%lu\n", n);
  for (i = 0; i < n; i++) {

    printf("Before: dest[%lu]=", i);
    print_src_dest(dest[i]);
    printf(" src[%lu]=", i);
    print_src_dest(src[i]);
    puts("");

    dest[i] = src[i];
    src[i] = NULL;

    printf("After: dest[%lu]=", i);
    print_src_dest(dest[i]);
    printf(" src[%lu]=", i);
    print_src_dest(src[i]);
    puts("");
    /* printf("After: dest[%lu]=%i, src[%lu]=%i\n", i, *(int *)dest[i], i, */
    /*        *(int *)src[i]); */
    /* puts(""); */
  }
}
