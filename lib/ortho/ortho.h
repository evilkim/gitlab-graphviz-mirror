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

#include <render.h>

#ifdef _WIN32
#ifdef GVC_EXPORTS
#define ORTHO_API __declspec(dllexport)
#else
#define ORTHO_API __declspec(dllimport)
#endif
#else
#define ORTHO_API /* nothing */
#endif

ORTHO_API void orthoEdges (Agraph_t* g, int useLbls);
