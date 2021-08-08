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

#include <ast/ast.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#include <ast/compat_unistd.h>
#endif
#include <stddef.h>
#include <stdint.h>

/*
 * return external representation for symbolic link text of name in buf
 * the link text string length is returned
 */

size_t pathgetlink(const char *name, char *buf, size_t siz)
{
#ifdef _WIN32
	return SIZE_MAX;
#else
    ssize_t n;

    if ((n = readlink(name, buf, siz)) < 0)
	return SIZE_MAX;
    if ((size_t)n >= siz) {
	errno = EINVAL;
	return SIZE_MAX;
    }
    buf[n] = 0;
    return (size_t)n;
#endif
}
