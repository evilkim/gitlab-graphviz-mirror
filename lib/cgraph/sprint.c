#include <assert.h>
#include <cgraph/sprint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __clang__
#define NONNULL _Nonnull
#define NULLABLE _Nullable
#else
#define NONNULL /* nothing */
#define NULLABLE /* nothing */
#endif

static char *NULLABLE print(const char *NONNULL format, va_list ap) {
  assert(format != NULL);

  va_list ap2;
  va_copy(ap2, ap);

  // how many bytes do we need to construct this string?
  int r = vsnprintf(NULL, 0, format, ap2);
  va_end(ap2);
  if (r < 0) {
    // bad format string
    return NULL;
  }
  size_t len = (size_t)r + 1 /* for NUL */;

  // allocate space to construct the string
  char *s = malloc(len);
  if (s == NULL) {
    return NULL;
  }

  // construct it
  (void)vsnprintf(s, len, format, ap);

  return s;
}

char *NULLABLE gv_sprint(const char *NONNULL format, ...) {
  assert(format != NULL);

  va_list ap;
  va_start(ap, format);
  char *s = print(format, ap);
  va_end(ap);
  return s;
}

char *NONNULL gv_sprint_or_exit(const char *NONNULL format, ...) {
  assert(format != NULL);

  va_list ap;
  va_start(ap, format);
  char *s = print(format, ap);
  va_end(ap);

  if (s == NULL) {
    fprintf(stderr, "gv_sprint failed\n");
    exit(EXIT_FAILURE);
  }

  return s;
}
