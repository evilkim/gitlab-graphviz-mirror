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

#include "viewport.h"

#ifdef __cplusplus
extern "C" {
#endif

    void glmotion_zoom(void);
    void glmotion_pan(ViewInfo * v);
    void glmotion_zoom_inc(int zoomin);

#ifdef __cplusplus
}				/* end extern "C" */
#endif
