#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

#include "darray.h"

/* ── Test harness ─────────────────────────────────────────────────── */

static int passed = 0;
static int failed = 0;

#define CHECK(label, cond)                                            \
   do {                                                               \
      if (cond) {                                                     \
         printf("  [PASS] %s\n", label);                              \
         passed++;                                                     \
      } else {                                                        \
         printf("  [FAIL] %s  (line %d)\n", label, __LINE__);         \
         failed++;                                                     \
      }                                                               \
   } while(0)

#define SECTION(name) printf("\n=== %s ===\n", name)

/* ── Types under test ─────────────────────────────────────────────── */

typedef struct {
   i32 x;
   i32 y;
   f64 val;
} Vec2;

DARRAY_INIT(f64)
DARRAY_INIT(i32)
DARRAY_INIT(char)
DARRAY_INIT(Vec2)

   /* ── Helpers ──────────────────────────────────────────────────────── */

   static i32 rand_i32(void) {
      return (i32)((rand() % 200001) - 100000);
   }

static f64 rand_f64(void) {
   return ((f64)rand() / RAND_MAX) * 200000.0 - 100000.0;
}

static char rand_char(void) {
   return (char)(33 + rand() % 94);
}

static Vec2 rand_vec2(void) {
   return (Vec2){ rand_i32(), rand_i32(), rand_f64() };
}

/* ── i32 tests ────────────────────────────────────────────────────── */

static void test_i32_push_and_at(void) {
   SECTION("i32 | push + at");
   Darray_i32 d = i32_new();

   CHECK("empty size is 0",        d.size == 0);
   CHECK("empty capacity is 0",    d.capacity == 0);
   CHECK("empty node is NULL",     d.node == NULL);

   CHECK("push 0",                 i32_push(&d, 0));
   CHECK("push INT32_MAX",         i32_push(&d, 2147483647));
   CHECK("push INT32_MIN",         i32_push(&d, -2147483648));
   CHECK("push -1",                i32_push(&d, -1));
   CHECK("size is 4",              d.size == 4);
   CHECK("at(0) == 0",             i32_at(&d, 0) == 0);
   CHECK("at(1) == INT32_MAX",     i32_at(&d, 1) == 2147483647);
   CHECK("at(2) == INT32_MIN",     i32_at(&d, 2) == -2147483648);
   CHECK("at(3) == -1",            i32_at(&d, 3) == -1);

   for (i32 i = 0; i < 1000; i++) CHECK("push random", i32_push(&d, rand_i32()));
   CHECK("size after 1000 random pushes", d.size == 1004);
   CHECK("capacity >= size",       d.capacity >= d.size);

   i32_free(&d);
   CHECK("node NULL after free",   d.node == NULL);
   CHECK("size 0 after free",      d.size == 0);
   CHECK("capacity 0 after free",  d.capacity == 0);
}

static void test_i32_pop(void) {
   SECTION("i32 | pop");
   Darray_i32 d = i32_new();

   i32_push(&d, 10);
   i32_push(&d, 20);
   i32_push(&d, 30);

   CHECK("pop returns last (30)",  i32_pop(&d) == 30);
   CHECK("size decremented",       d.size == 2);
   CHECK("pop returns next (20)",  i32_pop(&d) == 20);
   CHECK("pop returns next (10)",  i32_pop(&d) == 10);
   CHECK("size is 0",              d.size == 0);

   i32_free(&d);
}

static void test_i32_strip(void) {
   SECTION("i32 | strip");
   Darray_i32 d = i32_new();

   for (i32 i = 0; i < 10; i++) i32_push(&d, i);

   i32_strip(&d, 3);
   CHECK("strip 3 from 10 -> size 7",  d.size == 7);

   i32_strip(&d, 0);
   CHECK("strip 0 -> size unchanged",  d.size == 7);

   i32_strip(&d, 100);
   CHECK("strip > size -> size 0",     d.size == 0);

   i32_strip(&d, 0);
   CHECK("strip 0 on empty -> size 0", d.size == 0);

   i32_free(&d);
}

static void test_i32_append(void) {
   SECTION("i32 | append");
   Darray_i32 d = i32_new();

   CHECK("append to empty",        i32_append(&d, 99));
   CHECK("size is 1",              d.size == 1);
   CHECK("at(0) == 99",            i32_at(&d, 0) == 99);

   i32_push(&d, 1);
   i32_push(&d, 2);
   CHECK("append prepends",        i32_append(&d, 55));
   CHECK("at(0) == 55",            i32_at(&d, 0) == 55);
   CHECK("at(1) == 99",            i32_at(&d, 1) == 99);
   CHECK("size is 4",              d.size == 4);

   for (i32 i = 0; i < 500; i++) i32_append(&d, rand_i32());
   CHECK("capacity >= size after 500 appends", d.capacity >= d.size);

   i32_free(&d);
}

static void test_i32_insert_at(void) {
   SECTION("i32 | insert_at");
   Darray_i32 d = i32_new();

   i32_push(&d, 1);
   i32_push(&d, 2);
   i32_push(&d, 3);

   CHECK("insert_at 0 (prepend)",  i32_insert_at(&d, 0, 0));
   CHECK("at(0) == 0",             i32_at(&d, 0) == 0);
   CHECK("at(1) == 1",             i32_at(&d, 1) == 1);

   CHECK("insert_at middle",       i32_insert_at(&d, 2, 99));
   CHECK("at(2) == 99",            i32_at(&d, 2) == 99);
   CHECK("at(3) == 2",             i32_at(&d, 3) == 2);

   CHECK("insert_at end (== size)",i32_insert_at(&d, d.size, 77));
   CHECK("last element == 77",     i32_at(&d, d.size - 1) == 77);

   CHECK("size correct",           d.size == 6);

   i32_free(&d);
}

static void test_i32_remove(void) {
   SECTION("i32 | remove");
   Darray_i32 d = i32_new();

   for (i32 i = 0; i < 5; i++) i32_push(&d, i);

   i32_remove(&d, 0);
   CHECK("remove first -> at(0)==1",  i32_at(&d, 0) == 1);
   CHECK("size decremented",          d.size == 4);

   i32_remove(&d, d.size - 1);
   CHECK("remove last -> size 3",     d.size == 3);
   CHECK("last element == 3",         i32_at(&d, d.size - 1) == 3);

   i32_remove(&d, 1);
   CHECK("remove middle -> size 2",   d.size == 2);
   CHECK("at(0)==1, at(1)==3",        i32_at(&d, 0) == 1 && i32_at(&d, 1) == 3);

   i32_free(&d);
}

static void test_i32_replace(void) {
   SECTION("i32 | replace");
   Darray_i32 d = i32_new();

   i32_push(&d, 1);
   i32_push(&d, 2);
   i32_push(&d, 3);

   i32_replace(&d, 0, -999);
   CHECK("replace first",          i32_at(&d, 0) == -999);

   i32_replace(&d, 2, 2147483647);
   CHECK("replace last with MAX",  i32_at(&d, 2) == 2147483647);

   i32_replace(&d, 1, 0);
   CHECK("replace middle with 0",  i32_at(&d, 1) == 0);

   i32_free(&d);
}

static void test_i32_reserve(void) {
   SECTION("i32 | reserve");
   Darray_i32 d = i32_new();

   CHECK("reserve 0 on empty",     i32_reserve(&d, 0));
   CHECK("capacity still 0",       d.capacity == 0);

   CHECK("reserve 100",            i32_reserve(&d, 100));
   CHECK("capacity >= 100",        d.capacity >= 100);

   CHECK("reserve smaller noop",   i32_reserve(&d, 10));
   CHECK("capacity unchanged",     d.capacity >= 100);

   for (i32 i = 0; i < 100; i++) i32_push(&d, i);
   CHECK("no realloc needed after reserve", d.capacity >= 100);

   i32_free(&d);
}

static void test_i32_shrink(void) {
   SECTION("i32 | shrink");
   Darray_i32 d = i32_new();

   for (i32 i = 0; i < 100; i++) i32_push(&d, i);
   size_t cap_before = d.capacity;
   i32_strip(&d, 90);
   CHECK("capacity unchanged after strip", d.capacity == cap_before);

   CHECK("shrink succeeds",        i32_shrink(&d));
   CHECK("capacity == size after shrink", d.capacity == d.size);
   CHECK("size still 10",          d.size == 10);
   CHECK("data intact after shrink", i32_at(&d, 0) == 0 && i32_at(&d, 9) == 9);

   i32_strip(&d, 10);
   CHECK("shrink on size==0 frees node", i32_shrink(&d) && d.node == NULL && d.capacity == 0);

   i32_free(&d);
}

static void test_i32_at_ptr(void) {
   SECTION("i32 | at_ptr");
   Darray_i32 d = i32_new();

   i32_push(&d, 42);
   i32* ptr = i32_at_ptr(&d, 0);
   CHECK("at_ptr returns correct address", ptr == &d.node[0]);
   *ptr = 999;
   CHECK("write through ptr visible via at", i32_at(&d, 0) == 999);

   i32_free(&d);
}

static void test_i32_unsafe_at(void) {
   SECTION("i32 | unsafe_at");
   Darray_i32 d = i32_new();

   i32_push(&d, -7);
   i32_push(&d, 13);
   CHECK("unsafe_at(0) == -7",     i32_unsafe_at(&d, 0) == -7);
   CHECK("unsafe_at(1) == 13",     i32_unsafe_at(&d, 1) == 13);

   i32_free(&d);
}

/* ── f64 tests ────────────────────────────────────────────────────── */

static void test_f64_push_pop(void) {
   SECTION("f64 | push + pop");
   Darray_f64 d = f64_new();

   f64 vals[] = { 0.0, -0.0, 1.5, -1.5, 1e308, -1e308, 0.0000001 };
   int n = (int)(sizeof(vals) / sizeof(vals[0]));

   for (int i = 0; i < n; i++) f64_push(&d, vals[i]);
   CHECK("size matches pushed count", d.size == (size_t)n);

   for (int i = n - 1; i >= 0; i--) {
      f64 v = f64_pop(&d);
      CHECK("pop returns correct f64 value", v == vals[i]);
   }
   CHECK("empty after all pops", d.size == 0);

   for (int i = 0; i < 1000; i++) f64_push(&d, rand_f64());
   CHECK("capacity >= size after 1000 random f64 pushes", d.capacity >= d.size);

   f64_free(&d);
}

static void test_f64_insert_remove(void) {
   SECTION("f64 | insert_at + remove");
   Darray_f64 d = f64_new();

   f64_push(&d, 1.1);
   f64_push(&d, 3.3);
   f64_insert_at(&d, 1, 2.2);

   CHECK("insert_at middle f64",   d.size == 3);
   CHECK("at(0)==1.1",             f64_at(&d, 0) == 1.1);
   CHECK("at(1)==2.2",             f64_at(&d, 1) == 2.2);
   CHECK("at(2)==3.3",             f64_at(&d, 2) == 3.3);

   f64_remove(&d, 1);
   CHECK("remove middle f64",      d.size == 2);
   CHECK("at(0)==1.1",             f64_at(&d, 0) == 1.1);
   CHECK("at(1)==3.3",             f64_at(&d, 1) == 3.3);

   f64_free(&d);
}

/* ── char tests ───────────────────────────────────────────────────── */

static void test_char_push_append(void) {
   SECTION("char | push + append");
   Darray_char d = char_new();

   char str[] = "hello";
   for (int i = 0; str[i]; i++) char_push(&d, str[i]);
   CHECK("size == 5",              d.size == 5);
   CHECK("at(0)=='h'",             char_at(&d, 0) == 'h');
   CHECK("at(4)=='o'",             char_at(&d, 4) == 'o');

   char_append(&d, '!');
   CHECK("append prepends '!'",    char_at(&d, 0) == '!');
   CHECK("size == 6",              d.size == 6);

   char_push(&d, '\0');
   CHECK("push null terminator",   char_at(&d, d.size - 1) == '\0');

   for (int i = 0; i < 500; i++) char_push(&d, rand_char());
   CHECK("capacity >= size after random chars", d.capacity >= d.size);

   char_free(&d);
}

static void test_char_strip_shrink(void) {
   SECTION("char | strip + shrink");
   Darray_char d = char_new();

   for (int i = 0; i < 50; i++) char_push(&d, rand_char());
   char_strip(&d, 25);
   CHECK("strip 25 -> size 25",    d.size == 25);

   CHECK("shrink -> capacity==size", char_shrink(&d) && d.capacity == 25);

   char_strip(&d, 1000);
   CHECK("strip > size -> 0",      d.size == 0);

   char_free(&d);
}

/* ── Vec2 struct tests ────────────────────────────────────────────── */

static void test_vec2_push_at(void) {
   SECTION("Vec2 | push + at");
   Darray_Vec2 d = Vec2_new();

   Vec2 a = {0, 0, 0.0};
   Vec2 b = {-1, -1, -1.0};
   Vec2 c = {2147483647, -2147483648, 1e308};

   Vec2_push(&d, a);
   Vec2_push(&d, b);
   Vec2_push(&d, c);

   CHECK("size == 3",              d.size == 3);
   CHECK("at(0).x == 0",          Vec2_at(&d, 0).x == 0);
   CHECK("at(1).y == -1",         Vec2_at(&d, 1).y == -1);
   CHECK("at(2).val == 1e308",     Vec2_at(&d, 2).val == 1e308);

   for (int i = 0; i < 500; i++) Vec2_push(&d, rand_vec2());
   CHECK("capacity >= size after 500 Vec2 pushes", d.capacity >= d.size);

   Vec2_free(&d);
}

static void test_vec2_replace_remove(void) {
   SECTION("Vec2 | replace + remove");
   Darray_Vec2 d = Vec2_new();

   for (int i = 0; i < 5; i++) Vec2_push(&d, (Vec2){i, i, (f64)i});

   Vec2 replacement = {99, 99, 99.0};
   Vec2_replace(&d, 2, replacement);
   CHECK("replace middle struct",  Vec2_at(&d, 2).x == 99 && Vec2_at(&d, 2).y == 99);

   Vec2_remove(&d, 0);
   CHECK("remove first -> at(0).x==1", Vec2_at(&d, 0).x == 1);
   CHECK("size after remove",      d.size == 4);

   Vec2_remove(&d, d.size - 1);
   CHECK("remove last -> size 3",  d.size == 3);

   Vec2_free(&d);
}

static void test_vec2_append_insert(void) {
   SECTION("Vec2 | append + insert_at");
   Darray_Vec2 d = Vec2_new();

   Vec2_push(&d, (Vec2){1, 1, 1.0});
   Vec2_push(&d, (Vec2){3, 3, 3.0});

   Vec2_insert_at(&d, 1, (Vec2){2, 2, 2.0});
   CHECK("insert_at 1 struct",     Vec2_at(&d, 1).x == 2);
   CHECK("at(2).x==3 after insert", Vec2_at(&d, 2).x == 3);

   Vec2_append(&d, (Vec2){0, 0, 0.0});
   CHECK("append struct prepends", Vec2_at(&d, 0).x == 0);
   CHECK("size == 4",              d.size == 4);

   Vec2_free(&d);
}

static void test_vec2_pop_shrink(void) {
   SECTION("Vec2 | pop + shrink");
   Darray_Vec2 d = Vec2_new();

   for (int i = 0; i < 10; i++) Vec2_push(&d, (Vec2){i, i, (f64)i});

   Vec2 last = Vec2_pop(&d);
   CHECK("pop returns last Vec2",  last.x == 9 && last.y == 9);
   CHECK("size 9 after pop",       d.size == 9);

   Vec2_strip(&d, 9);
   CHECK("strip all -> size 0",    d.size == 0);
   CHECK("shrink frees on empty",  Vec2_shrink(&d) && d.node == NULL);

   Vec2_free(&d);
}

/* ── Growth factor verification ───────────────────────────────────── */

static void test_growth_sequence(void) {
   SECTION("growth | capacity sequence from 0");
   Darray_i32 d = i32_new();

   size_t last_cap = 0;
   bool growth_ok = true;

   for (int i = 0; i < 10000; i++) {
      i32_push(&d, i);
      if (d.capacity != last_cap) {
         if (last_cap >= 8) {
            double ratio = (double)d.capacity / (double)last_cap;
            if (ratio < 1.4 || ratio > 1.7) {
               growth_ok = false;
               printf("  [INFO] unexpected growth ratio %.3f at cap %zu -> %zu\n",
                     ratio, last_cap, d.capacity);
            }
         }
         last_cap = d.capacity;
      }
   }

   CHECK("growth ratio stays near 1.5x", growth_ok);
   CHECK("capacity >= size at 10000",    d.capacity >= d.size);

   i32_free(&d);
}

/* ── Reserve edge cases ───────────────────────────────────────────── */

static void test_reserve_edge(void) {
   SECTION("reserve | edge cases");
   Darray_i32 d = i32_new();

   CHECK("reserve 1 on empty",     i32_reserve(&d, 1));
   CHECK("capacity == 1",          d.capacity == 1);
   CHECK("size still 0",           d.size == 0);

   CHECK("reserve same size noop", i32_reserve(&d, 1) && d.capacity == 1);
   CHECK("reserve 0 noop",         i32_reserve(&d, 0) && d.capacity == 1);

   i32_push(&d, 42);
   CHECK("push after reserve works", d.size == 1 && i32_at(&d, 0) == 42);

   i32_free(&d);
}

/* ── Multi-operation sequence ─────────────────────────────────────── */

static void test_mixed_sequence(void) {
   SECTION("mixed | interleaved push/append/insert/remove/pop");
   Darray_i32 d = i32_new();

   for (i32 i = 0; i < 20; i++) i32_push(&d, i);
   i32_append(&d, -1);
   i32_append(&d, -2);
   CHECK("after push+append size==22", d.size == 22);
   CHECK("at(0)==-2",              i32_at(&d, 0) == -2);
   CHECK("at(1)==-1",              i32_at(&d, 1) == -1);

   i32_insert_at(&d, 5, 999);
   CHECK("insert_at 5 size==23",   d.size == 23);
   CHECK("at(5)==999",             i32_at(&d, 5) == 999);

   i32_remove(&d, 5);
   CHECK("remove at 5 size==22",   d.size == 22);

   i32 v = i32_pop(&d);
   CHECK("pop returns 19",         v == 19);
   CHECK("size==21 after pop",     d.size == 21);

   i32_strip(&d, 21);
   CHECK("strip all",              d.size == 0);

   i32_shrink(&d);
   CHECK("shrink after strip all frees node", d.node == NULL && d.capacity == 0);

   CHECK("push after shrink+strip works", i32_push(&d, 1) && d.size == 1);

   i32_free(&d);
}

/* ── Entry point ──────────────────────────────────────────────────── */

int main(void) {
   srand((u32)time(NULL));

   test_i32_push_and_at();
   test_i32_pop();
   test_i32_strip();
   test_i32_append();
   test_i32_insert_at();
   test_i32_remove();
   test_i32_replace();
   test_i32_reserve();
   test_i32_shrink();
   test_i32_at_ptr();
   test_i32_unsafe_at();

   test_f64_push_pop();
   test_f64_insert_remove();

   test_char_push_append();
   test_char_strip_shrink();

   test_vec2_push_at();
   test_vec2_replace_remove();
   test_vec2_append_insert();
   test_vec2_pop_shrink();

   test_growth_sequence();
   test_reserve_edge();
   test_mixed_sequence();

   printf("\n────────────────────────────────\n");
   printf("  passed: %d\n", passed);
   printf("  failed: %d\n", failed);
   printf("────────────────────────────────\n");

   return failed > 0 ? 1 : 0;
}
