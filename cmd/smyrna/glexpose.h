/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef GLEXPOSE_H
#define GLEXPOSE_H

#include "viewport.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* int glupdatecamera(ViewInfo * v); */
    int glexpose_main(ViewInfo * v);
    /* void glexpose_grid(ViewInfo * v); */
    /* int glexpose_drawgraph(ViewInfo * view); */
    void drawRotatingTools(void);
    /* void drawtestpoly(void); */
    /* void draw_cube(void); */

#ifdef __cplusplus
}				/* end extern "C" */
#endif
#endif
