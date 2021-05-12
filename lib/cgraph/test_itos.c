// basic unit tester for itos

#ifdef NDEBUG
#error this is not intended to be compiled with assertions off
#endif

#include <assert.h>
#include <cgraph/itos.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_0(void) {
  assert(strcmp(itos(0).str, "0") == 0);
}

static void test_1(void) {
  assert(strcmp(itos(1).str, "1") == 0);
}

static void test_neg_1(void) {
  assert(strcmp(itos(-1).str, "-1") == 0);
}

static void test_min(void) {

  int r = snprintf(NULL, 0, "%d", INT_MIN);
  assert(r > 0);

  char *buffer = malloc(sizeof(char) * ((size_t)r + 1));
  assert(buffer != NULL);

  (void)sprintf(buffer, "%d", INT_MIN);

  assert(strcmp(itos(INT_MIN).str, buffer) == 0);

  free(buffer);
}

static void test_max(void) {

  int r = snprintf(NULL, 0, "%d", INT_MAX);
  assert(r > 0);

  char *buffer = malloc(sizeof(char) * ((size_t)r + 1));
  assert(buffer != NULL);

  (void)sprintf(buffer, "%d", INT_MAX);

  assert(strcmp(itos(INT_MAX).str, buffer) == 0);

  free(buffer);
}

int main(void) {

#define RUN(t)                                                                 \
  do {                                                                         \
    printf("running test_%s... ", #t);                                         \
    fflush(stdout);                                                            \
    test_##t();                                                                \
    printf("OK\n");                                                            \
  } while (0)

  RUN(0);
  RUN(1);
  RUN(neg_1);
  RUN(min);
  RUN(max);

#undef RUN

  return EXIT_SUCCESS;
}
