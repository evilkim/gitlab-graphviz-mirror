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

#ifndef XLAYOUT_H
#define XLAYOUT_H

#include <fdpgen/fdp.h>

    typedef struct {
	int numIters;
	double T0;
	double K;
	double C;
	int loopcnt;
    } xparams;

    extern void fdp_xLayout(graph_t *, xparams *);

#endif

#ifdef __cplusplus
}
#endif
