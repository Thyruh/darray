#pragma once

#include <stdlib.h>
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

typedef bool b32;

typedef float f32;
typedef double f64;

#define DARRAY_INIT(T)                                                                                                 \
   typedef struct {                                                                                                    \
      T* node;                                                                                                         \
      u32 size;                                                                                                        \
      u32 capacity;                                                                                                    \
   } Darray_##T;                                                                                                       \
static inline b32 T##_push(Darray_##T* darray, T item) {                                                               \
   if (darray->capacity == 0) {                                                                                        \
      darray->capacity = 8;                                                                                            \
      darray->node = malloc(sizeof(T) * darray->capacity);                                                             \
   }                                                                                                                   \
   if (darray->size >= darray->capacity) {                                                                             \
      darray->capacity *= 2;                                                                                           \
      T* tmp = realloc(darray->node, sizeof(T) * darray->capacity);                                                    \
      if (!tmp) return false;                                                                                          \
      darray->node = tmp;                                                                                              \
   }                                                                                                                   \
   darray->node[darray->size++] = item;                                                                                \
   return true;                                                                                                        \
}                                                                                                                      \
static inline void T##_pop(Darray_##T* darray, u32 count) {                                                            \
   if (count > darray->size) darray->size = 0;                                                                         \
   else darray->size -= count;                                                                                         \
}                                                                                                                      \
static inline T* T##_at(Darray_##T* darray, u32 index)  {                                                              \
   return &darray->node[index];                                                                                        \
}                                                                                                                      \
static inline T* T##_safe_at(Darray_##T* darray, u32 index)  {  /* Beware of segfaults after calling this */           \
   if (index >= darray->size) return NULL;                                                                             \
   return &darray->node[index];                                                                                        \
}                                                                                                                      \
static inline b32 T##_reserve(Darray_##T* darray, u32 block)  {                                                        \
   if (block <= darray->capacity) return true;                                                                         \
   T* tmp = realloc(darray->node, sizeof(T) * block);                                                                  \
   if (!tmp) return false;                                                                                             \
   darray->node = tmp;                                                                                                 \
   darray->capacity = block;                                                                                           \
   return true;                                                                                                        \
}                                                                                                                      \
static inline Darray_##T T##_new() {                                                                                   \
   return (Darray_##T) {NULL, 0, 0};                                                                                   \
}                                                                                                                      \
static inline void T##_free(Darray_##T* darray) {                                                                      \
   free(darray->node);                                                                                                 \
   *darray = T##_new();                                                                                                \
}
