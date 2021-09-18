/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifdef FEATURE_CS
#include <ast.h>
#endif
/*
 * Copyright 1989 Software Research Associates, Inc., Tokyo, Japan
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Software Research Associates not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  Software Research Associates
 * makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * SOFTWARE RESEARCH ASSOCIATES DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL SOFTWARE RESEARCH ASSOCIATES BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Erik M. van der Poel
 *         Software Research Associates, Inc., Tokyo, Japan
 *         erik@sra.co.jp
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif

#ifdef SEL_FILE_IGNORE_CASE
#include <ctype.h>
#endif /* def SEL_FILE_IGNORE_CASE */

#include "SFinternal.h"

#include <dirent.h>

#include <sys/stat.h>

#include <stdlib.h>

#include "SFDecls.h"

int SFcompareEntries (const void *vp, const void *vq) {
    const SFEntry *p = vp, *q = vq;
#ifdef SEL_FILE_IGNORE_CASE
#ifdef _MSC_VER
    return _stricmp(p->real, q->real);
#else
    return strcasecmp(p->real, q->real);
#endif
#else /* def SEL_FILE_IGNORE_CASE */
    return strcmp (p->real, q->real);
#endif /* def SEL_FILE_IGNORE_CASE */
}

int SFgetDir (SFDir *dir) {
    SFEntry       *result = NULL;
    int           alloc = 0;
    int           i;
    DIR           *dirp;
    struct dirent *dp;
    char          *str;
    int           len;
    int           maxChars;
    struct stat   statBuf;

    maxChars = strlen (dir->dir) - 1;
    dir->entries = NULL;
    dir->nEntries = 0;
    dir->nChars = 0;
    result = NULL;
    i = 0;
    dirp = opendir (".");
    if (!dirp) {
        return 1;
    }
    stat (".", &statBuf);
    dir->mtime = statBuf.st_mtime;
    readdir (dirp);    /* throw away "." */

#ifndef S_IFLNK
    readdir (dirp);    /* throw away ".." */
#endif /* ndef S_IFLNK */

    while ((dp = readdir (dirp))) {
        if (i >= alloc) {
            alloc = 2 * (alloc + 1);
            result = (SFEntry *) XtRealloc (
                (char *) result, (unsigned) (alloc * sizeof (SFEntry))
            );
        }
        result[i].statDone = 0;
        str = dp->d_name;
        len = strlen (str);
        result[i].real = XtMalloc ((unsigned) (len + 2));
        strcat (strcpy (result[i].real, str), " ");
        if (len > maxChars) {
            maxChars = len;
        }
        result[i].shown = result[i].real;
        i++;
    }

    qsort(result, (size_t)i, sizeof(SFEntry), SFcompareEntries);

    dir->entries = result;
    dir->nEntries = i;
    dir->nChars = maxChars + 1;
    closedir (dirp);
    return 0;
}
