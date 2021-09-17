/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_SEARCH_H
#include <search.h>
#endif
#include <ctype.h>
typedef struct hsbcolor_t {
    char *name;
    unsigned char h, s, b;
} hsbcolor_t;


#ifndef NOCOLORNAMES
#include "colortbl.h"

static char *canoncolor(const char *orig, char *out)
{
    char c;
    char *p = out;
    while ((c = *orig++)) {
	if (!isalnum(c))
	    continue;
	if (isupper(c))
	    c = (char)tolower(c);
	*p++ = c;
    }
    *p = '\0';
    return out;
}

static int colorcmpf(const void *a0, const void *a1)
{
    const hsbcolor_t *p0 = a0;
    const hsbcolor_t *p1 = a1;
    return strcmp(p0->name, p1->name);
}

char *colorxlate(char *str, char *buf)
{
    static hsbcolor_t *last;
    char canon[128];
    char *p;
    hsbcolor_t fake;

    if (last == NULL || strcmp(last->name, str)) {
	fake.name = canoncolor(str, canon);
	last = bsearch(&fake, color_lib, sizeof(color_lib) / sizeof(hsbcolor_t),
	               sizeof(fake), colorcmpf);
    }
    if (last == NULL) {
	if (!isdigit(canon[0])) {
	    fprintf(stderr, "warning: %s is not a known color\n", str);
	    strcpy(buf, str);
	} else
	    for (p = buf; (*p = *str++); p++)
		if (*p == ',')
		    *p = ' ';
    } else
	sprintf(buf, "%.3f %.3f %.3f", ((double) last->h) / 255,
		((double) last->s) / 255, ((double) last->b) / 255);
    return buf;
}
#else
char *colorxlate(char *str, char *buf)
{
    return str;
}
#endif
