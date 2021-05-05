#pragma once

#include <stdio.h>

// return type of itos below
struct itos_ {
  char str[12];
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
