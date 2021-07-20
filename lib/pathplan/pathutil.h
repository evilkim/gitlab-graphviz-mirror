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

#define _BLD_pathplan 1

#include "pathplan.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NOT
#define NOT(x)	(!(x))
#endif
#ifndef FALSE
#define FALSE	0
#define TRUE	(NOT(FALSE))
#endif

/*visual studio*/
#ifdef _WIN32
#ifndef PATHPLAN_EXPORTS
#define PATHUTIL_API __declspec(dllimport)
#endif
#endif
/*end visual studio*/
#ifndef PATHUTIL_API
#define PATHUTIL_API extern
#endif
	typedef double COORD;
    PATHUTIL_API COORD area2(Ppoint_t, Ppoint_t, Ppoint_t);
    PATHUTIL_API int wind(Ppoint_t a, Ppoint_t b, Ppoint_t c);
    PATHUTIL_API COORD dist2(Ppoint_t, Ppoint_t);

    int in_poly(Ppoly_t argpoly, Ppoint_t q);

#undef PATHUTIL_API
#ifdef __cplusplus
}
#endif
