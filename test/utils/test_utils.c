/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <stdio.h>

#include "ar_list.h"
#include "test_utils.h"

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
void print_array_pointers(ar_list l) {
  size_t i;
  puts("{");
  for (i = 0; i < l.size; i++) {
    printf("%p,\n", l.array[i]);
  }
  puts("}");
}

void print_array_values(ar_list l) {
  size_t i;
  puts("{");
  for (i = 0; i < l.size; i++) {
    printf("%i,\n", *(int *)l.array[i]);
  }
  puts("}");
}

void print_size_and_capacity(ar_list l) {
  printf("Size: %lu, Capacity: %lu\n", l.size, l.capacity);
}
