/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cgraph/agxbuf.h>

#define N_GNEW(n,t)	 calloc((n),sizeof(t))

/* agxbinit:
 * Assume if init is non-null, hint = sizeof(init[])
 */
void agxbinit(agxbuf * xb, unsigned int hint, unsigned char *init)
{
    if (init) {
	xb->buf = init;
	xb->dyna = 0;
    } else {
	if (hint == 0)
	    hint = BUFSIZ;
	xb->dyna = 1;
	xb->buf = N_GNEW(hint, unsigned char);
    }
    xb->eptr = xb->buf + hint;
    xb->ptr = xb->buf;
    *xb->ptr = '\0';
}

/* agxbmore;
 * Expand buffer to hold at least ssz more bytes.
 */
int agxbmore(agxbuf * xb, size_t ssz)
{
    size_t cnt = 0;         /* current no. of characters in buffer */
    size_t size = 0;        /* current buffer size */
    size_t nsize = 0;       /* new buffer size */
    unsigned char *nbuf;    /* new buffer */

    size = (size_t) (xb->eptr - xb->buf);
    nsize = 2 * size;
    if (size + ssz > nsize)
	nsize = size + ssz;
    cnt = (size_t) (xb->ptr - xb->buf);
    if (xb->dyna) {
	nbuf = realloc(xb->buf, nsize);
    } else {
	nbuf = N_GNEW(nsize, unsigned char);
	memcpy(nbuf, xb->buf, cnt);
	xb->dyna = 1;
    }
    xb->buf = nbuf;
    xb->ptr = xb->buf + cnt;
    xb->eptr = xb->buf + nsize;
    return 0;
}

int agxbprint(agxbuf * xb, const char *fmt, ...) {
  va_list ap;
  size_t size;
  int result;

  va_start(ap, fmt);

  /* determine how many bytes we need to print */
  {
    va_list ap2;
    int rc;
    va_copy(ap2, ap);
    rc = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (rc < 0) {
      va_end(ap);
      return rc;
    }
    size = (size_t)rc + 1; /* account for NUL terminator */
  }

  /* do we need to expand the buffer? */
  {
    size_t unused_space = (size_t)(xb->eptr - xb->ptr);
    if (unused_space < size) {
      size_t extra = size - unused_space;
      (void)agxbmore(xb, extra);
    }
  }

  /* we can now safely print into the buffer */
  result = vsnprintf((char*)xb->ptr, size, fmt, ap);
  assert(result == (int)(size - 1) || result < 0);
  if (result > 0) {
    xb->ptr += (size_t)result;
  }

  va_end(ap);
  return result;
}

/* agxbput_n:
 * Append string s of length n onto xb
 */
size_t agxbput_n(agxbuf * xb, const char *s, size_t ssz)
{
    if (xb->ptr + ssz > xb->eptr)
	agxbmore(xb, ssz);
    memcpy(xb->ptr, s, ssz);
    xb->ptr += ssz;
    return ssz;
}

/* agxbput:
 * Append string s into xb
 */
size_t agxbput(agxbuf * xb, const char *s)
{
    size_t ssz = strlen(s);

    return agxbput_n(xb, s, ssz);
}

/* agxbfree:
 * Free any malloced resources.
 */
void agxbfree(agxbuf * xb)
{
    if (xb->dyna)
	free(xb->buf);
}

/* agxbpop:
 * Removes last character added, if any.
 */
int agxbpop(agxbuf * xb)
{
    int c;
    if (xb->ptr > xb->buf) {
	c = *xb->ptr--;
	return c;
    } else
	return -1;

}

char *agxbdisown(agxbuf * xb) {

  size_t size;
  char *buf;

  /* terminate the existing string */
  agxbputc(xb, '\0');

  size = (size_t)(xb->ptr - xb->buf);

  if (!xb->dyna) {
    /* the buffer is not dynamically allocated, so we need to copy its contents
     * to heap memory
     */

    buf = malloc(size);
    if (buf == NULL) {
      return NULL;
    }

    memcpy(buf, xb->buf, size);

  } else {
    /* the buffer is already dynamically allocated, so take it as-is */
    buf = (char*)xb->buf;
  }

  /* reset xb to a state where it is usable */
  xb->buf = xb->ptr = xb->eptr = NULL;
  xb->dyna = 1;

  return buf;
}
