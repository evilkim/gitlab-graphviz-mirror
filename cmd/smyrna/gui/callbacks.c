/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE 1
#include "config.h"
#include <gtk/gtk.h>
#include "callbacks.h"
#include "viewport.h"
#include "selectionfuncs.h"
#include <common/memory.h>

//Menu Items 

void new_graph_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    g_print("new graph button fired\n");
}

void open_graph_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;
}

void save_graph_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    if (view->activeGraph > -1)
	save_graph_with_file_name(view->g[view->activeGraph], NULL);
}

void save_as_graph_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Save File",
					 NULL,
					 GTK_FILE_CHOOSER_ACTION_SAVE,
					 GTK_STOCK_CANCEL,
					 GTK_RESPONSE_CANCEL,
					 GTK_STOCK_SAVE,
					 GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER
						   (dialog), TRUE);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
	char *filename;
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	save_graph_with_file_name(view->g[view->activeGraph], filename);
	g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void remove_graph_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    g_print("remove graph button fired\n");
}

static void btn_clicked(GtkWidget * widget, gvk_layout layout)
{
    (void)widget;
    (void)layout;
}

void btn_dot_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)user_data;

    btn_clicked(widget, GVK_DOT);
}

void btn_neato_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)user_data;

    btn_clicked(widget, GVK_NEATO);
}

void btn_twopi_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)user_data;

    btn_clicked(widget, GVK_TWOPI);
}

void btn_circo_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)user_data;

    btn_clicked(widget, GVK_CIRCO);
}

void btn_fdp_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)user_data;

    btn_clicked(widget, GVK_FDP);
}

void on_newNode(GtkWidget * button, gpointer user_data)
{
   (void)button;
   (void)user_data;
}

void dlgOpenGraph_OK_Clicked(GtkWidget * button, gpointer data)
{
    (void)button;

    g_print("ok is pressed - %i was pressed\n", *(int *) data);
}

//dlgOpenGraph btnOK clicked
void on_dlgOpenGraph_btnOK_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    if (update_graph_properties(view->g[view->activeGraph]))
	gtk_dialog_response((GtkDialog *)
			    glade_xml_get_widget(xml, "dlgOpenGraph"),
			    GTK_RESPONSE_OK);
}

//dlgOpenGraph btncancelclicked
void on_dlgOpenGraph_btncancel_clicked(GtkWidget * widget,
				       gpointer user_data)
{
    (void)widget;
    (void)user_data;

    gtk_dialog_response((GtkDialog *)
			glade_xml_get_widget(xml, "dlgOpenGraph"),
			GTK_RESPONSE_CANCEL);
}

void attr_widgets_modifiedSlot(GtkWidget * widget, gpointer user_data)
{
    (void)widget;

    attr_widgets_modified[*(int *) user_data] = 1;
    g_print("attr changed signal..incoming data : %i\n",
	    *(int *) user_data);
}

void frmObject_set_scroll(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    g_print("scroll resize required\n");
}

void frmObjectBtnOK_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    deselect_all(view->g[view->activeGraph]);
    gtk_widget_hide(glade_xml_get_widget(xml, "frmObject"));
}

void frmObjectBtnCancel_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    gtk_widget_hide(glade_xml_get_widget(xml, "frmObject"));
}

/*console output widgets*/
_BB void on_clearconsolebtn_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    gtk_text_buffer_set_text(gtk_text_view_get_buffer
			     ((GtkTextView *)
			      glade_xml_get_widget(xml, "mainconsole")),
			     "", 0);
}

_BB void on_hideconsolebtn_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    gtk_widget_hide(glade_xml_get_widget(xml, "vbox13"));
}

_BB void on_consoledecbtn_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    int w, h;
    gtk_widget_get_size_request((GtkWidget*)
				glade_xml_get_widget(xml,
						     "scrolledwindow7"),
				&w, &h);
    w -= 5;
    gtk_widget_set_size_request(((GtkWidget*)
				 glade_xml_get_widget(xml,
						      "scrolledwindow7")),
				w, 0);
}

_BB void on_consoleincbtn_clicked(GtkWidget * widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;

    int w, h;
    gtk_widget_get_size_request((GtkWidget*)
				glade_xml_get_widget(xml,
						     "scrolledwindow7"),
				&w, &h);
    w += 5;
    gtk_widget_set_size_request(((GtkWidget*)
				 glade_xml_get_widget(xml,
						      "scrolledwindow7")),
				w, 0);
}
