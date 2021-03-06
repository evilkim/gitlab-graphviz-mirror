/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Lefteris Koutsofios - AT&T Bell Laboratories */

#ifndef _COMMON_H
#define _COMMON_H
#ifdef HAVECS
#include <ast.h>
#else
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <sys/select.h>
#include <math.h>
#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

#define POS __FILE__, __LINE__

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#ifndef L_SUCCESS
#define L_SUCCESS 1
#define L_FAILURE 0
#endif

#define CHARSRC 0
#define FILESRC 1

    extern int warnflag;
    extern char *leftypath, *leftyoptions, *shellpath;
    extern jmp_buf exitljbuf;
    extern int idlerunmode;
    extern fd_set inputfds;

    int init(char *);
    void term(void);
    char *buildpath(char *, int);
    char *buildcommand(char *, char *, int, int, char *);
    void warning(char *, int, char *, char *, ...);

#ifdef __GNUC__
  // FIXME: use _Noreturn for all compilers when we move to C11
  #define NORETURN __attribute__((noreturn))
#else
  #define NORETURN /* nothing */
#endif
    NORETURN void panic1(char *, int, char *, char *, ...);
    NORETURN void panic2(char *, int, char *, char *, ...);
#undef NORETURN

#endif				/* _COMMON_H */

#ifdef __cplusplus
}
#endif
