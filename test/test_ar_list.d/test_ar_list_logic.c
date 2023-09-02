/*******************************************************************************
 *    IMPORTS
 ******************************************************************************/
#include <stdlib.h>

#include <unity.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "ar_list.h"
#include "mock_std_lib_utils.h"

/*******************************************************************************
 *    TESTS DECLARATIONS
 ******************************************************************************/
int *l_values;
size_t l_values_size;

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
int arl_small_values[] = {0, 1, 2, 3, 4, 5};

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

void test_unity(void) { TEST_PASS_MESSAGE("Unity is working."); }

void test_arl_init_failure(void) {
  ar_list l, *received;

  app_malloc_IgnoreAndReturn(NULL);
  get_pointer_size_ExpectAndReturn(sizeof(void));

  received = arl_init(&l, 10);

  TEST_ASSERT_NULL(received);
}

/*******************************************************************************
 *    PRIVATE API TESTS
 ******************************************************************************/

/* void test_arl_alloc_array */
