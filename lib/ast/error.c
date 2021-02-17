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
 * standalone mini error implementation
 */

#ifdef _WIN32
#include <config.h>

#ifdef GVDLL
#define _BLD_sfio 1
#endif
#endif

#include <ast/ast.h>
#include <ast/error.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

Error_info_t error_info;

void setErrorLine (int line) { error_info.line = line; }
void setErrorFileLine (char* src, int line) {
    error_info.file = src;
    error_info.line = line;
}
void setErrorId (char* id) { error_info.id = id; }
void setErrorErrors (int errors) { error_info.errors = errors; }
int  getErrorErrors () { return error_info.errors; }
void setTraceLevel (int i) { error_info.trace = i; }

void errorv(const char *id, int level, const char *s, va_list ap)
{
    int flags;

    if (level < error_info.trace) return;
    if (level < 0)
	flags = 0;
    else {
	flags = level & ~ERROR_LEVEL;
	level &= ERROR_LEVEL;
    }
    if (level && ((s = error_info.id) || (s = id))) {
	if (flags & ERROR_USAGE)
	    sfprintf(sfstderr, "Usage: %s ", s);
	else
	    sfprintf(sfstderr, "%s: ", s);
    }
    if (flags & ERROR_USAGE)
	/*nop */ ;
    else if (level < 0) {
	int i;
	for (i = 0; i < error_info.indent; i++)
	    sfprintf(sfstderr, "  ");
	sfprintf(sfstderr, "debug%d: ", level);
    } else if (level) {
	if (level == ERROR_WARNING) {
	    sfprintf(sfstderr, "warning: ");
	    error_info.warnings++;
	} else {
	    error_info.errors++;
	    if (level == ERROR_PANIC)
		sfprintf(sfstderr, "panic: ");
	}
	if (error_info.line) {
	    if (error_info.file && *error_info.file)
		sfprintf(sfstderr, "\"%s\", ", error_info.file);
	    sfprintf(sfstderr, "line %d: ", error_info.line);
	}
    }
    sfvprintf(sfstderr, s, ap);
    if (flags & ERROR_SYSTEM)
	sfprintf(sfstderr, "\n%s", strerror(errno));
    sfprintf(sfstderr, "\n");
    if (level >= ERROR_FATAL)
	exit(level - ERROR_FATAL + 1);
}

void error(int level, const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    errorv(NULL, level, s, ap);
    va_end(ap);
}

void errorf(void *handle, void *discipline, int level, const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    errorv((discipline
	    && handle) ? *((char **) handle) : (char *) handle, level, s, ap);
    va_end(ap);
}
