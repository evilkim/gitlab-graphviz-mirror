#pragma once

#include <assert.h>
#include <stdlib.h>

/** Marker for a point in code which execution can never reach.
 *
 * As a C11 function, this could be thought of as:
 *
 *   _Noreturn void UNREACHABLE(void);
 *
 * Calling this teaches the compiler that the call site can never be executed,
 * and it can optimize with this assumption in mind. This can be used to explain
 * that a switch is exhaustive:
 *
 *   switch (…) {
 *   default: UNREACHABLE();
 *   …remaining cases that cover all possibilities…
 *   }
 *
 * or that a function coda can be omitted:
 *
 *   int foo(void) {
 *     while (always_true()) {
 *     }
 *     UNREACHABLE();
 *   }
 */
#ifdef __GNUC__
#define UNREACHABLE()                                                          \
  do {                                                                         \
    assert(0 && "unreachable");                                                \
    __builtin_unreachable();                                                   \
  } while (0)
#elif defined(_MSC_VER)
#define UNREACHABLE()                                                          \
  do {                                                                         \
    assert(0 && "unreachable");                                                \
    __assume(0);                                                               \
  } while (0)
#else
#define UNREACHABLE()                                                          \
  do {                                                                         \
    assert(0 && "unreachable");                                                \
    abort();                                                                   \
  } while (0)
#endif
