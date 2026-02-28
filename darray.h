#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define DARRAY_INIT(T)                                                                                                 \
   typedef struct {                                                                                                    \
      T* node;                                                                                                         \
      size_t size;                                                                                                     \
      size_t capacity;                                                                                                 \
   } Darray_##T;                                                                                                       \
static inline bool T##_push(Darray_##T* darray, T item) {                                                              \
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
static inline void T##_pop(Darray_##T* darray, size_t count) {                                                         \
   if (count > darray->size) darray->size = 0;                                                                         \
   else darray->size -= count;                                                                                         \
}                                                                                                                      \
static inline T* T##_unsafe_at(Darray_##T* darray, size_t index) {                                                     \
   return &darray->node[index];                                                                                        \
}                                                                                                                      \
static inline T* T##_at(Darray_##T* darray, size_t index)  {  /* Beware of segfaults after calling this */             \
   if (index >= darray->size) return NULL;                                                                             \
   return &darray->node[index];                                                                                        \
}                                                                                                                      \
static inline bool T##_reserve(Darray_##T* darray, size_t block)  {                                                    \
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
