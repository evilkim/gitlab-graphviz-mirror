/* basic unit tester for vmalloc */

#ifdef NDEBUG
#error this is not intended to be compiled with assertions off
#endif

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include vmalloc and some of its internals directly so we can call them */
#include <vmalloc/vmhdr.h>
#include <vmalloc/vmalloc.h>
#include <vmalloc/vmbest.c>
#include <vmalloc/vmclear.c>
#include <vmalloc/vmclose.c>
#include <vmalloc/vmopen.c>
#include <vmalloc/vmstrdup.c>

/* trivial lifecycle of a vmalloc */
static void test_basic_lifecycle(void) {
  int r;

  /* create a new vmalloc region */
  Vmalloc_t *v = vmopen();

  /* this program does not allocate much, so vmopen() should not have failed */
  assert(v != NULL);

  /* close the region */
  r = vmclose(v);
  assert(r == 0);
}

/* vmclear should be fine to call repeatedly on an empty region */
static void test_empty_vmclear(void) {
  int r;

  /* create a new vmalloc region */
  Vmalloc_t *v = vmopen();
  assert(v != NULL);

  /* we should be able to clear this repeatedly with no effect */
  r = vmclear(v);
  assert(r == 0);
  assert(v->size == 0);
  r = vmclear(v);
  assert(r == 0);
  assert(v->size == 0);
  r = vmclear(v);
  assert(r == 0);
  assert(v->size == 0);

  /* close the region */
  r = vmclose(v);
  assert(r == 0);
}

/* a more realistic lifecycle usage of a vmalloc */
static void test_lifecycle(void) {
  int r;
  size_t i, j;

  /* some arbitrary sizes we will allocate */
  static const size_t allocations[] = {123,  1,    42,   2,    3,
                                       4096, 1024, 1023, 20000};

  /* length of the above array */
  enum { allocations_len = sizeof(allocations) / sizeof(allocations[0]) };

  /* somewhere to store pointers to memory we allocate */
  unsigned char *p[allocations_len];

  /* create a new vmalloc region */
  Vmalloc_t *v = vmopen();
  assert(v != NULL);

  for (i = 0; i < allocations_len; ++i) {

    /* create the allocation */
    p[i] = vmalloc(v, allocations[i]);
    assert(p[i] != NULL);

    /* confirm we can write to it without faulting */
    for (j = 0; j < allocations[i]; ++j) {
      volatile unsigned char *q = (volatile unsigned char *)&p[i][j];
      *q = (unsigned char)(j % UCHAR_MAX);
    }

    /* confirm we can read back what we wrote */
    for (j = 0; j < allocations[i]; ++j) {
      volatile unsigned char *q = (volatile unsigned char *)&p[i][j];
      assert(*q == (unsigned char)(j % UCHAR_MAX));
    }
  }

  /* the allocator should have a correct account of what we have allocated */
  assert(v->size == allocations_len);

  /* free a few allocations out of order from that in which we allocated */
  r = vmfree(v, p[3]);
  assert(r == 0);
  r = vmfree(v, p[6]);
  assert(r == 0);
  r = vmfree(v, p[5]);
  assert(r == 0);

  /* the allocator should have correctly stopped tracking those pointers */
  assert(v->size == allocations_len - 3);

  /* free the rest of the allocations in one sweep */
  r = vmclear(v);
  assert(r == 0);

  /* the allocator should have dropped all pointers it was tracking */
  assert(v->size == 0);

  /* try the allocate-write-read test in reverse order */
  for (i = allocations_len - 1;; --i) {

    /* create the allocation */
    p[i] = vmalloc(v, allocations[i]);
    assert(p[i] != NULL);

    /* confirm we can write to it without faulting */
    for (j = 0; j < allocations[i]; ++j) {
      volatile unsigned char *q = (volatile unsigned char *)&p[i][j];
      *q = (unsigned char)(j % UCHAR_MAX);
    }

    /* confirm we can read back what we wrote */
    for (j = 0; j < allocations[i]; ++j) {
      volatile unsigned char *q = (volatile unsigned char *)&p[i][j];
      assert(*q == (unsigned char)(j % UCHAR_MAX));
    }

    if (i == 0) {
      break;
    }
  }

  /* clean up */
  r = vmclose(v);
  assert(r == 0);
}

/* some testing of our realloc analogue */
static void test_resize(void) {
  int r;
  unsigned char *p;
  static const size_t s = 10;
  size_t i;

  /* create a new vmalloc region */
  Vmalloc_t *v = vmopen();
  assert(v != NULL);

  /* resizing NULL should give us a new pointer */
  p = vmresize(v, NULL, s);
  assert(p != NULL);

  /* confirm we can read and write to this allocated memory */
  for (i = 0; i < s; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i];
    *q = (unsigned char)i;
  }
  for (i = 0; i < s; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i];
    assert(*q == (unsigned char)i);
  }

  /* discard this */
  r = vmfree(v, p);
  assert(r == 0);

  /* get a new pointer */
  p = vmalloc(v, s);
  assert(p != NULL);

  /* setup some data in this memory */
  for (i = 0; i < s; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i];
    *q = (unsigned char)(s - i);
  }

  /* expand the memory */
  p = vmresize(v, p, s * 2);
  assert(p != NULL);

  /* confirm our previous data is still intact */
  for (i = 0; i < s; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i];
    assert(*q == (unsigned char)(s - i));
  }

  /* confirm we can access the extended part */
  for (i = 0; i < s; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i + s];
    *q = (unsigned char)(s - i);
  }

  /* shrink the memory */
  p = vmresize(v, p, s / 2);
  assert(p != NULL);

  /* confirm the part that we retained still has its data intact */
  for (i = 0; i < s / 2; ++i) {
    volatile unsigned char *q = (volatile unsigned char *)&p[i];
    assert(*q == (unsigned char)(s - i));
  }

  /* clean up */
  r = vmclose(v);
  assert(r == 0);
}

/* basic test of our strdup analogue */
static void test_strdup(void) {
  char *s, *t;
  const char *sc;
  int r;

  /* create a new vmalloc region */
  Vmalloc_t *v = vmopen();
  assert(v != NULL);

  /* vmstrdup does not assume the source pointer came from its vmalloc, so lets
   * use a string in ordinary heap memory
   */
  s = strdup("hello world");
  assert(s != NULL);

  /* ask vmalloc to strdup this */
  t = vmstrdup(v, s);
  assert(t != NULL);

  /* we should have received back the duplicated content */
  assert(strcmp(s, t) == 0);

  /* discard the original string */
  free(s);

  /* now ask vmalloc to strdup something it produced itself */
  s = vmstrdup(v, t);
  assert(s != NULL);

  /* we should still have the correct original string content */
  assert(strcmp(s, t) == 0);

  /* discard these strings */
  r = vmfree(v, s);
  assert(r == 0);
  r = vmfree(v, t);
  assert(r == 0);

  /* vmstrdup does not assume the input string is mutable, so lets pass it a
   * non-writable string
   */
  sc = "foo bar";
  t = vmstrdup(v, sc);
  assert(t != NULL);

  /* we should have received back the correct content */
  assert(strcmp(sc, t) == 0);

  /* clean up */
  r = vmclose(v);
  assert(r == 0);
}

int main(void) {

#define RUN(t)                                                                 \
  do {                                                                         \
    printf("running test_%s... ", #t);                                         \
    fflush(stdout);                                                            \
    test_##t();                                                                \
    printf("OK\n");                                                            \
  } while (0)

  RUN(basic_lifecycle);
  RUN(empty_vmclear);
  RUN(lifecycle);
  RUN(resize);
  RUN(strdup);

#undef RUN

  return EXIT_SUCCESS;
}
