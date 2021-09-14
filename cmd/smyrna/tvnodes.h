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

#include "gui.h"
#define LOCATION_X_CHKSELECTED 16
#define LOCATION_X_IDLABEL 45
#define LOCATION_X_CHKVISIBLE 139
#define LOCATION_X_CHKHIGHLIGHTED 202
#define LOCATION_X_NAME  276
#define LOCATION_X_DATA1 356
#define LOCATION_X_DATA2 561

#ifdef __cplusplus
extern "C" {
#endif

    int tv_show_all(void);
    int tv_hide_all(void);
    int tv_save_as(int);
    void setup_tree (Agraph_t* g);

#ifdef __cplusplus
}				/* end extern "C" */
#endif
