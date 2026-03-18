# darray.h — v0.0.1
A type-generic dynamic array for C, generated via macros.

## Setup
Call `DARRAY_INIT(T)` once per type you want to use, at file scope.
This generates a struct `Darray_<T>` and all associated functions prefixed with `<T>_`.
```c
DARRAY_INIT(int)   →   Darray_int, int_push(), int_pop(), etc.
```

## A word on removal
Most operations that appear to "remove" elements don't actually free or overwrite anything — they just decrement size, making those slots inaccessible through the normal interface. The memory is still there.

If the function name contains the word **remove**, it does a real delete. Everything else is just a size adjustment.

## API Reference

**`<T>_new()`**
Returns a new zero-initialized `Darray_<T>`. Start here.

**`<T>_free(Darray_<T>*)`**
Frees the internal buffer and zeroes the struct. Call this when done.

**`<T>_push(Darray_<T>*, T new)`**
Appends `new` to the end (at index `size`). Grows if needed.

**`<T>_pop(Darray_<T>*)`**
Returns the last element and decrements size. Does not free. Like the instruction.

**`<T>_append(Darray_<T>*, T new)`**
Inserts `new` at index 0, shifting everything else right.
This is a prepend — named append for symmetry with push. Be aware of the O(n) cost.

**`<T>_insert_at(Darray_<T>*, size_t index, T new)`**
Inserts `new` at `index`, shifting everything from `index` onward right.

**`<T>_at(Darray_<T>*, size_t index)`**
Returns the element at `index`. Bounds-checked — calls `abort()` on out-of-bounds access.

**`<T>_at_ptr(Darray_<T>*, size_t index)`**
Same as `_at` but returns a pointer to the element. Bounds-checked.

**`<T>_unsafe_at(Darray_<T>*, size_t index)`**
Returns the element at `index` with no bounds checking. Faster, but undefined behavior if `index >= size`. Only use this if you've already validated the index.

**`<T>_replace(Darray_<T>*, size_t index, T new)`**
Overwrites the element at `index` with `new`.

**`<T>_strip(Darray_<T>*, size_t block)`**
Does `size -= block`. A bulk pop. Does not free memory.

**`<T>_reserve(Darray_<T>*, size_t block)`**
Ensures `capacity >= block`. Reallocates only if current capacity is insufficient. Use this to avoid repeated small allocations.

**`<T>_shrink(Darray_<T>*)`**
Sets `capacity = size`. Frees unused memory. Call after you're done growing the array and want a tight allocation.

**`<T>_grow(Darray_<T>*)`** *(internal — do not call)*
Forces an immediate reallocation. Called automatically by push and insert. Calling it manually is safe but pointless unless you're poking at the internals for a reason.

## DARRAY_BIND
`DARRAY_BIND(<T>, name)` creates a global `Darray_<T>` named `name` and generates thin wrapper functions so you can drop the pointer argument.
```c
DARRAY_BIND(int, nums)

→  nums_push(10)     instead of    int_push(&nums, 10)
→  nums_pop()        instead of    int_pop(&nums)
```

The tradeoff: it's global state. Fine for small programs or quick scripts. For anything that needs multiple independent arrays of the same type, skip BIND and use the raw interface.

## Example
```c
#include "darray.h"

DARRAY_INIT(int)
DARRAY_BIND(int, ints)

int main(void) {
    ints_push(10);        // bound interface — no pointer needed
    ints_push(20);

    Darray_int local = int_new();   // raw interface — fully manual
    int_push(&local, 99);
    int_free(&local);

    return 0;
}
```

There is no built-in print function. Even for int. You'll figure it out.


## To compile use make
```
make
```
This will compile all the .c files (even custom ones you might create) in the directory into separate binaries
