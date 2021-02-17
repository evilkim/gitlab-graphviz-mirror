/* platform abstraction for case-insensitive string functions */

#pragma once

#include <stddef.h>

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
