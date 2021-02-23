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
 * AT&T Research
 *
 * expression library
 */

#include <expr/exlib.h>

/*
 * 0 terminate string and optionally vmstrdup() return value
 */

char*
exstash(Sfio_t* sp, Vmalloc_t* vp)
{
	char*	s;

	return ((s = sfstruse(sp)) && (!vp || (s = vmstrdup(vp, s)))) ? s : exnospace();
}
