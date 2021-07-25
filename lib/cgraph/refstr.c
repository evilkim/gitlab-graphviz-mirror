/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <cgraph/cghdr.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * reference counted strings.
 */

typedef struct {
    Dtlink_t link;
    uint64_t refcnt: sizeof(uint64_t) * 8 - 1;
    uint64_t is_html: 1;
    char *s;
    char store[1];		/* this is actually a dynamic array */
} refstr_t;

/** Compare two reference counted strings.
 *
 * \param container Ignored.
 * \param a First refstr_t to compare.
 * \param b Second refstr_t to compare.
 * \param discipline Ignored.
 * \return a strcmp-like return value indicating the ordering of the two
 *   strings.
 */
static int cmp(Dt_t *container, void *a, void *b, Dtdisc_t *discipline) {

  (void)container;
  (void)discipline;

  const refstr_t *x = a;
  const refstr_t *y = b;

  // consider all regular strings to precede HTML-like strings
  if (x->is_html && !y->is_html) {
    return 1;
  }
  if (!x->is_html && y->is_html) {
    return -1;
  }

  return strcmp(x->s, y->s);
}

/** Derive a hash value for a reference counted string.
 *
 * \param container Ignored.
 * \param a A refstr_t to hash.
 * \param discipline Ignored.
 * \return A numeric hash digest of this string.
 */
static unsigned hash(Dt_t *container, void *a, Dtdisc_t *discipline) {

  (void)container;
  (void)discipline;

  const refstr_t *x = a;

  // use the HTML-ness of this string as a seed input to ensure it is accounted
  // for in the hash
  unsigned seed = (unsigned)x->is_html;

  // delegate to CDT’s default string hashing
  return dtstrhash(seed, x->s, -1);
}

static Dtdisc_t Refstrdisc = {
    0, // key offset
    sizeof(refstr_t), // size of key
    0,				/* link offset */
    NULL,
    agdictobjfree,
    cmp,
    hash,
    agdictobjmem,
    NULL
};

static Dict_t *Refdict_default;

/* refdict:
 * Return the string dictionary associated with g.
 * If necessary, create it.
 * As a side-effect, set html masks. This assumes 8-bit bytes.
 */
static Dict_t *refdict(Agraph_t * g)
{
    Dict_t **dictref;

    if (g)
	dictref = &(g->clos->strdict);
    else
	dictref = &Refdict_default;
    if (*dictref == NULL) {
	*dictref = agdtopen(g, &Refstrdisc, Dttree);
    }
    return *dictref;
}

int agstrclose(Agraph_t * g)
{
    return agdtclose(g, refdict(g));
}

static refstr_t *refsymbind(Dict_t * strdict, const char *s, bool is_html)
{
    refstr_t key = {0}, *r;
// Suppress Clang/GCC -Wcast-qual warning. Casting away const here is acceptable
// as dtsearch does not modify its input key.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
    key.s = (char*)s;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    key.is_html = is_html;
    r = dtsearch(strdict, &key);
    return r;
}

static char *refstrbind(Dict_t * strdict, const char *s, bool is_html)
{
    refstr_t *r;
    r = refsymbind(strdict, s, is_html);
    if (r)
	return r->s;
    else
	return NULL;
}

char *agstrbind(Agraph_t * g, const char *s)
{
  // try to bind a regular string first
  char *r = refstrbind(refdict(g), s, /* is_html = */ false);
  if (r != NULL) {
    return r;
  }

  // if that failed, try to bind an HTML-like string
  return refstrbind(refdict(g), s, /* is_html = */ true);
}

char *agstrdup(Agraph_t * g, const char *s)
{
    refstr_t *r;
    Dict_t *strdict;
    size_t sz;

    if (s == NULL)
	 return NULL;
    strdict = refdict(g);
    r = refsymbind(strdict, s, /* is_html = */ false);
    if (r)
	r->refcnt++;
    else {
	sz = sizeof(refstr_t) + strlen(s);
	if (g)
	    r = agalloc(g, sz);
	else
	    r = malloc(sz);
	r->refcnt = 1;
	r->is_html = 0;
	strcpy(r->store, s);
	r->s = r->store;
	dtinsert(strdict, r);
    }
    return r->s;
}

char *agstrdup_html(Agraph_t * g, const char *s)
{
    refstr_t *r;
    Dict_t *strdict;
    size_t sz;

    if (s == NULL)
	 return NULL;
    strdict = refdict(g);
    r = refsymbind(strdict, s, /* is_html = */ true);
    if (r)
	r->refcnt++;
    else {
	sz = sizeof(refstr_t) + strlen(s);
	if (g)
	    r = agalloc(g, sz);
	else
	    r = malloc(sz);
	r->refcnt = 1;
	r->is_html = 1;
	strcpy(r->store, s);
	r->s = r->store;
	dtinsert(strdict, r);
    }
    return r->s;
}

int agstrfree(Agraph_t * g, const char *s)
{
    refstr_t *r;
    Dict_t *strdict;

    if (s == NULL)
	 return FAILURE;

    strdict = refdict(g);
    // first look for a regular string, then for an HTML-like string
    for (bool is_html = false; ; is_html = !is_html) {
      r = refsymbind(strdict, s, is_html);
      if (r && r->s == s) {
        // found a match
        r->refcnt--;
        if (r->refcnt == 0) {
          agdtdelete(g, strdict, r);
        }
        break;
      }
      if (is_html) {
        break;
      }
    }
    if (r == NULL)
	return FAILURE;
    return SUCCESS;
}

/* aghtmlstr:
 * Return true if s is an HTML string.
 * We assume s points to the datafield store[0] of a refstr.
 */
int aghtmlstr(const char *s)
{
    const refstr_t *key;

    if (s == NULL)
	return 0;
    key = (const refstr_t *) (s - offsetof(refstr_t, store[0]));
    return key->is_html;
}

#ifdef DEBUG
static int refstrprint(Dict_t * dict, void *ptr, void *user)
{
    refstr_t *r;

    NOTUSED(dict);
    r = ptr;
    NOTUSED(user);
    fprintf(stderr, "%s\n", r->s);
    return 0;
}

void agrefstrdump(Agraph_t * g)
{
    dtwalk(refdict(g), refstrprint, 0);
}
#endif
