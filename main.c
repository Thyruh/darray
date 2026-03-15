#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#include "darray.h"

DARRAY_INIT(i32)
DARRAY_BIND(i32, i32s)

int main(void) {
   for (u32 i = 0; i <= 10000; i++) i32s_push(i);

   i32s_append(-1);
   i32s_append(-2);
   i32s_append(10);

   printf("-------------------------------------- Full darray of integers ----------------------------------\n");

   printf("elements: ");
   for (u32 i = 0; i < i32s.size-1; i++) {
      printf("%d, ", i32s_at(i));
   }
   printf("%d.\n", i32s_at(i32s.size-1));
   printf("length = %zu\n", i32s.size);

   printf("-------------------------------------- Popped darray of integers ----------------------------------\n");
   
   printf("POPPED 10 ELEMENTS\n");

   printf("elements: ");
   for (u32 i = 0; i < i32s.size-1; i++) {
      i32 element = i32s_at(i);
      printf("%d, ", element);
   }
   printf("%d.\n", i32s_at(i32s.size-1));
   printf("length = %zu\n", i32s.size);

   printf("-------------------------------------- Unsafe access ----------------------------------\n");

   printf("elements: ");
   for (u32 i = 0; i < 120; i++) {
      printf("%d, ", i32s_unsafe_at(i));
   }
   printf("%d.\n", i32s_unsafe_at(120));
   printf("length = %zu\n", i32s.size);

   i32s_free();

   return 0;
}
