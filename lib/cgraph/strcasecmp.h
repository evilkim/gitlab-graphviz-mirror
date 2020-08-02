/* platform abstraction for case-insensitive string functions */

#pragma once

#include <stddef.h>

/* Re-prototype the functions we are abstracting. The purpose of this is to
 * cause a compiler warning if our implementations diverge. Windows compilation
 * will see these and the inline functions below, and warn if they do not match.
 * Compilation on other platforms will see these and the strings.h prototypes,
 * and warn if they do not match. This way we have a safeguard that the
 * alternatives provided for Windows match the libc functions on other
 * platforms.
 */
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

#ifdef _MSC_VER
  /* redirect these to the Windows alternatives */

  #include <string.h>

  static inline int strcasecmp(const char *s1, const char *s2) {
    return _stricmp(s1, s2);
  }

  static inline int strncasecmp(const char *s1, const char *s2, size_t n) {
    return _strnicmp(s1, s2, n);
  }

#else
  /* other platforms define these in strings.h */
  #include <strings.h>

#endif
