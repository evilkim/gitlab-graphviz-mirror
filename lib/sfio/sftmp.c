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

#include	<sfio/sfhdr.h>
#include	<stddef.h>

char **_sfgetpath(char *path)
{
    char *p, **dirs;
    int n;

    if (!(path = getenv(path)))
	return NULL;

    for (p = path, n = 0;;) {	/* count number of directories */
	while (*p == ':')
	    ++p;
	if (*p == 0)
	    break;
	n += 1;
	while (*p && *p != ':')	/* skip dir name */
	    ++p;
    }
    if (n == 0 || !(dirs = malloc((n + 1) * sizeof(char *))))
	return NULL;
    if (!(p = malloc(strlen(path) + 1))) {
	free(dirs);
	return NULL;
    }
    strcpy(p, path);
    for (n = 0;; ++n) {
	while (*p == ':')
	    ++p;
	if (*p == 0)
	    break;
	dirs[n] = p;
	while (*p && *p != ':')
	    ++p;
	if (*p == ':')
	    *p++ = 0;
    }
    dirs[n] = NULL;

    return dirs;
}
