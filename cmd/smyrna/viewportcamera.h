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

#include "smyrnadefs.h"
#include <glcomp/glcompset.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern void attach_camera_widget(ViewInfo * view);
    extern void menu_click_add_camera(void *p);

#ifdef __cplusplus
}				/* end extern "C" */
#endif
