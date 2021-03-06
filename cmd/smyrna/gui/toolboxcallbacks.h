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

#include <gtk/gtk.h>
#include "gui.h"
#if defined(_WIN32)
#define _BB  __declspec(dllexport)
#else
#define _BB /**/
#endif
#ifdef __cplusplus
extern "C" {
#endif

    _BB void btnToolSingleSelect_clicked(GtkWidget * widget,
					 gpointer user_data);
    _BB void btnToolRectSelect_clicked(GtkWidget * widget,
				       gpointer user_data);
    _BB void btnToolRectXSelect_clicked(GtkWidget * widget,
					gpointer user_data);

    _BB void btnToolAntiRectSelect_clicked(GtkWidget * widget,
					   gpointer user_data);
    _BB void btnToolAntiRectXSelect_clicked(GtkWidget * widget,
					    gpointer user_data);



    _BB void btnToolPan_clicked(GtkWidget * widget, gpointer user_data);
    _BB void btnToolZoom_clicked(GtkWidget * widget, gpointer user_data);
    _BB void btnToolZoomIn_clicked(GtkWidget * widget, gpointer user_data);
    _BB void btnToolZoomOut_clicked(GtkWidget * widget,
				    gpointer user_data);
    _BB void btnToolZoomFit_clicked(GtkWidget * widget,
				    gpointer user_data);
    _BB void btnToolFit_clicked(GtkWidget * widget, gpointer user_data);
    _BB void btnToolMove_clicked(GtkWidget * widget, gpointer user_data);
    _BB void btnToolAddNode_clicked(GtkWidget * widget,
				    gpointer user_data);
    _BB void btnToolDeleteNode_clicked(GtkWidget * widget,
				       gpointer user_data);
    _BB void btnToolFindNode_clicked(GtkWidget * widget,
				     gpointer user_data);
    _BB void btnToolAddEdge_clicked(GtkWidget * widget,
				    gpointer user_data);
    _BB void btnToolDeleteEdge_clicked(GtkWidget * widget,
				       gpointer user_data);
    _BB void btnToolFindEdge_clicked(GtkWidget * widget,
				     gpointer user_data);
    _BB void btnToolCursor_clicked(GtkWidget * widget, gpointer user_data);
    _BB void on_btnActivateGraph_clicked(GtkWidget * widget,
					 gpointer user_data);

    void write_to_console(char *text);

#ifdef __cplusplus
}				/* end extern "C" */
#endif
