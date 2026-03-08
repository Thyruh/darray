#include <stdio.h>
#include <stdint.h>

typedef int32_t i32;

#include "darray.h"

DARRAY_INIT(i32);
DARRAY_BIND(i32, i32s);
DARRAY_BIND(i32, i32ss);

int main(void) {
   Darray_i32 vec_i32 = i32_new();
   i32s_push(10);
   i32ss_push(100);
   i32_push(&vec_i32, 1000);

   printf("%i.\n", *i32s_at(0));
   printf("%i.\n", *i32ss_at(0));
   printf("%i.\n\n\n", *i32_at(&vec_i32, 0));

   i32_push(&i32s, 20);
   i32_push(&i32ss, 200);
   i32_push(&vec_i32, 2000);

   printf("%i, %i.\n", *i32_at(&i32s, 0), *i32_at(&i32s, 1));
   printf("%i, %i.\n", *i32_at(&i32ss, 0), *i32_at(&i32ss, 1));
   printf("%i, %i.\n", *i32_at(&vec_i32, 0), *i32_at(&vec_i32, 1));

   i32s_free();
   i32ss_free();
   i32_free(&vec_i32);
   return 0;
}

// Output: 
//
// 10
// 100
// 1000
//
//
// 10, 20.
// 100, 200.
// 1000, 2000.
//
//
