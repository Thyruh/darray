#include <stdio.h>

#include "darray.h"

DARRAY_INIT(i32)
DARRAY_INIT(Darray_i32)

int main(void) {
   Darray_i32 i32s = i32_new();
   Darray_Darray_i32 v_i32s = Darray_i32_new();

   i32_reserve(&i32s, 100);
   for (u32 i = 0; i < 100; i++) i32_push(&i32s, i*2);

   printf("-------------------------------------- Darray of darrays ----------------------------------\n");

   Darray_i32_push(&v_i32s, i32s);
   printf("%d\n", *i32_at(Darray_i32_at(&v_i32s, 0), 2));

   printf("-------------------------------------- Full darray of integers ----------------------------------\n");

   printf("elements: ");
   for (u32 i = 0; i < i32s.size-1; i++) {
      printf("%d, ", *i32_at(&i32s, i));
   }
   printf("%d.\n", *i32_at(&i32s, i32s.size-1));
   printf("length = %d\n", i32s.size);

   i32_pop(&i32s, 10);

   printf("-------------------------------------- Popped darray of integers ----------------------------------\n");
   
   printf("[POPPED 10 ELEMENTS]\n");

   printf("elements: ");
   for (u32 i = 0; i < i32s.size-1; i++) {
      if (!i32_at(&i32s, i)) {
         fprintf(stderr, "[OUT OF BOUNDS ACCESS]: %s:%d:%s\n", __FILE__, __LINE__-1, " _at()");
         printf("\n");
         exit(1);
      }
      printf("%d, ", *i32_at(&i32s, i));
   }
   printf("%d.\n", *i32_at(&i32s, i32s.size-1));
   printf("length = %d\n", i32s.size);

   printf("-------------------------------------- Unsafe access ----------------------------------\n");

   printf("elements: ");
   for (u32 i = 0; i < 108; i++) {
      printf("%d, ", *i32_unsafe_at(&i32s, i));
   }
   printf("%d.\n", *i32_unsafe_at(&i32s, 108));
   printf("length = %d\n", i32s.size);

   i32_free(&i32s);
   Darray_i32_free(&v_i32s);

   return 0;
}
