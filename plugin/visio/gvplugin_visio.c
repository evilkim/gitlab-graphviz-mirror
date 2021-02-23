/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifdef _WIN32 //*dependencies
    #pragma comment( lib, "gvc.lib" )
    #pragma comment( lib, "graph.lib" )
#endif


#include <gvc/gvplugin.h>

extern gvplugin_installed_t gvdevice_vdx_types[];
extern gvplugin_installed_t gvrender_vdx_types[];

static gvplugin_api_t apis[] = {
    {API_device, gvdevice_vdx_types},
    {API_render, gvrender_vdx_types},
    {(api_t)0, 0},
};

#ifdef WIN32_DLL   /*visual studio*/
#ifndef GVPLUGIN_VISIO_EXPORTS
__declspec(dllimport) gvplugin_library_t gvplugin_visio_LTX_library = { "visio", apis };
#else
__declspec(dllexport) gvplugin_library_t gvplugin_visio_LTX_library = { "visio", apis };
#endif
#else /*end visual studio*/
#ifdef GVDLL
__declspec(dllexport) gvplugin_library_t gvplugin_visio_LTX_library = { "visio", apis };
#else
gvplugin_library_t gvplugin_visio_LTX_library = { "visio", apis };
#endif
#endif
