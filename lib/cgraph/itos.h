#pragma once

#include <stdio.h>

// maximum number of bytes needed to print a NUL-terminated int
enum { CHARS_FOR_NUL_TERM_INT = 12 };

// return type of itos below
struct itos_ {
  char str[CHARS_FOR_NUL_TERM_INT];
};

/** convert an integer to a string
 *
 * Intended usage is something like:
 *   agattr(g, AGEDGE, "hello", itos(42).str)
 *
 * @param i Integer to convert
 * @return Stringized conversion wrapped in a struct
 */
static inline struct itos_ itos(int i) {
  struct itos_ s;
  (void)snprintf(s.str, sizeof(s.str), "%d", i);
  return s;
}
