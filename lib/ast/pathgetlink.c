/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

/*
* Glenn Fowler
* AT&T Bell Laboratories
*/

#include "config.h"
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#include <ast/compat_unistd.h>
#endif

/*
 * return external representation for symbolic link text of name in buf
 * the link text string length is returned
 */

int pathgetlink(const char *name, char *buf, int siz)
{
#ifdef _WIN32
	return (-1);
#else
    int n;

    if ((n = readlink(name, buf, siz)) < 0)
	return (-1);
    if (n >= siz) {
	errno = EINVAL;
	return (-1);
    }
    buf[n] = 0;
    return (n);
#endif
}
