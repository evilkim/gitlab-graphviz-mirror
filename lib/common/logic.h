/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifndef NOT
#define NOT(v) (!(v))
#endif

#ifndef FALSE
#define	FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifndef NOTUSED
#define NOTUSED(var) (void) var
#endif

#ifndef NIL
#define NIL(type) ((type)0)
#endif

#ifdef __cplusplus
}
#endif
