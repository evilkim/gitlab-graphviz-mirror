// basic unit tester for gv_sprint

#ifdef NDEBUG
#error this is not intended to be compiled with assertions off
#endif

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// include sprint.c so we can be compiled standalone
#include <cgraph/sprint.c>

// print ""
static void test_empty_string(void) {
  char *s = gv_sprint("");
  assert(strcmp(s, "") == 0);
  free(s);
}

// print a basic string
static void test_simple(void) {
  char *s = gv_sprint("hello world");
  assert(strcmp(s, "hello world") == 0);
  free(s);
}

// print something large
static void test_large(void) {
  static const char text[] =
    "Sed ut perspiciatis, unde omnis iste natus error sit voluptatem "
    "accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab "
    "illo inventore veritatis et quasi architecto beatae vitae dicta sunt, "
    "explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut "
    "odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione "
    "voluptatem sequi nesciunt, neque porro quisquam est, qui do lorem ipsum, "
    "quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam "
    "[do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam "
    "quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] "
    "exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea "
    "commodi consequatur? [D]Quis autem vel eum iure reprehenderit, qui in ea "
    "voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui "
    "dolorem eum fugiat, quo voluptas nulla pariatur? [33] At vero eos et "
    "accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium "
    "voluptatum deleniti atque corrupti, quos dolores et quas molestias "
    "excepturi sint, obcaecati cupiditate non provident, similique sunt in "
    "culpa, qui officia deserunt mollitia animi, id est laborum et dolorum "
    "fuga. Et harum quidem rerum facilis est et expedita distinctio. Nam "
    "libero tempore, cum soluta nobis est eligendi optio, cumque nihil "
    "impedit, quo minus id, quod maxime placeat, facere possimus, omnis "
    "voluptas assumenda est, omnis dolor repellendus. Temporibus autem "
    "quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, "
    "ut et voluptates repudiandae sint et molestiae non recusandae. Itaque "
    "earum rerum hic tenetur a sapiente delectus, ut aut reiciendis "
    "voluptatibus maiores alias consequatur aut perferendis doloribus "
    "asperiores repellat.";
  char *s = gv_sprint(text);
  assert(strcmp(s, text) == 0);
  free(s);
}

// print some non-ASCII text
static void test_utf8(void) {
  static const char text[] = "Ça roule?";
  char *s = gv_sprint(text);
  assert(strcmp(s, text) == 0);
  free(s);
}

// print various numbers
static void test_int(void) {
  char *s = gv_sprint("%d is a nice number, more than %" PRId8
                      ", less than %lu", 7, INT8_C(-1), 42ul);
  assert(strcmp(s, "7 is a nice number, more than -1, less than 42") == 0);
  free(s);
}

// print some floating-point values
static void test_float(void) {
  char *s = gv_sprint("%.4f as a double or %.4f as a float, "
                      "can be truncated to %.3f", 1.2345, 1.2345f, 1.2345);
  assert(strcmp(s, "1.2345 as a double or 1.2345 as a float, "
                   "can be truncated to 1.234") == 0);
  free(s);
}

// escaping %
static void test_percent(void) {
  char *s = gv_sprint("this is a percent: %%.");
  assert(strcmp(s, "this is a percent: %.") == 0);
  free(s);
}

int main(void) {

#define RUN(t)                                                                 \
  do {                                                                         \
    printf("running test_%s... ", #t);                                         \
    fflush(stdout);                                                            \
    test_##t();                                                                \
    printf("OK\n");                                                            \
  } while (0)

  RUN(empty_string);
  RUN(simple);
  RUN(large);
  RUN(utf8);
  RUN(int);
  RUN(float);
  RUN(percent);

#undef RUN

  return EXIT_SUCCESS;
}
