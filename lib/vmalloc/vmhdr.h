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

/*	Common types, and macros for vmalloc functions.
**
**	Written by Kiem-Phong Vo, kpv@research.att.com, 01/16/94.
*/

#include <stdlib.h>
#include	<vmalloc/vmalloc.h>

void *bestalloc(Vmalloc_t * vm, size_t size);
int bestfree(Vmalloc_t * vm, void * data);
void *bestresize(Vmalloc_t * vm, void * data, size_t size);

#ifdef __cplusplus
}
#endif
