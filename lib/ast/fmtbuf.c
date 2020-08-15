/* $Id$ $Revision$ */
/* vim:set shiftwidth=4 ts=8: */

/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: See CVS logs. Details at http://www.graphviz.org/
 *************************************************************************/


#include <ast/ast.h>

/*
 * return small format buffer chunk of size n
 * format buffers are short lived
 */

static char buf[16 * 1024];
static char *nxt = buf;

char *fmtbuf(size_t n)
{
    char *cur;

    if (n > (&buf[elementsof(buf)] - nxt))
	nxt = buf;
    cur = nxt;
    nxt += n;
    return cur;
}
