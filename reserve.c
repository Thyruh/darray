#include "darray.h"

DARRAY_INIT(u32)

#define SIZE 1000000000

int main() {
   Darray_u32 u32s = u32_new();

   u32_reserve(&u32s,  SIZE); // 1 reallocation instead of log2(SIZE) allocations

   for (u64 i = 0; i < SIZE; i++) {
      u32_push(&u32s, i);
   }

   u32_free(&u32s);

   return 0;
}
