/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/


#include "config.h"
#include "builddate.h"
//windows.h for win machines
#if defined(_WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <windowsx.h>
#endif
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <glade/glade.h>
#include "gui.h"
#include "viewport.h"
#include "support.h"
#include "menucallbacks.h"
#include "gltemplate.h"
#include <common/memory.h>
#include "gvprpipe.h"
#include "frmobjectui.h"
#ifdef ENABLE_NLS
#include "libintl.h"
#endif
#include <assert.h>
#include "glexpose.h"
#include "glutrender.h"

#include <getopt.h>

#ifdef G_OS_WIN32
gchar *package_prefix;
gchar *package_data_dir;
#endif
gchar *package_locale_dir;
static char *smyrnaDir;		/* path to directory containin smyrna data files */
char *smyrnaGlade;
int width,height;/*glut window size*/


/* smyrnaPath:
 * Construct pathname for smyrna data file.
 * Base file name is given as suffix.
 * The function resolves the directory containing the data files,
 * and constructs a complete pathname.
 * The returned string is malloced, so the application should free
 * it later.
 * Returns NULL on error.
 */
char *smyrnaPath(char *suffix)
{
    char *buf;
    static int baselen;
    int slen;
#ifdef _WIN32
    char *pathSep = "\\";
#else
    char *pathSep = "/";
#endif
    assert(smyrnaDir);

    if (baselen == 0) {
	baselen = (int)strlen(smyrnaDir) + 2;
    }
    slen = (int)strlen(suffix);
    buf = N_NEW(baselen+slen, char);
    sprintf(buf, "%s%s%s", smyrnaDir, pathSep, suffix);
    return buf;
}

static char *useString = "Usage: smyrna [-v?] <file>\n\
  -f<WxH:bits@rate>         - full-screen mode\n\
  -e         - draw edges as splines if available\n\
  -v         - verbose\n\
  -?         - print usage\n";

static void usage(int v)
{
    fputs(useString, stdout);
    exit(v);
}

static char *Info[] = {
    "smyrna",                   /* Program */
    PACKAGE_VERSION,            /* Version */
    BUILDDATE                   /* Build Date */
};


static char *parseArgs(int argc, char *argv[], ViewInfo * view)
{
    int c;

    while ((c = getopt(argc, argv, ":eKf:txvV?")) != -1) {
	switch (c) {
	case 'e':
	    view->drawSplines = 1;
	    break;
	case 'v': // FIXME: deprecate and remove -v in future
	    break;
	case 'f':
	    view->guiMode=GUI_FULLSCREEN;
	    view->optArg=strdup(optarg);
	    break;

	case 'V':
	    fprintf(stderr, "%s version %s (%s)\n",
		    Info[0], Info[1], Info[2]);
	    exit (0);
	    break;
	case '?':
	    if (optopt == '\0')
		usage(0);
	    else {
		fprintf(stderr,
			"smyrna: option -%c unrecognized\n",
			optopt);
		usage(1);
	    }
	    break;
	}
    }

    if (optind < argc)
	return argv[optind];
    else
	return NULL;
}

static void windowedMode(int argc, char *argv[])
{
    GdkGLConfig *glconfig;
    /*combo box to show loaded graphs */
    GtkComboBox *graphComboBox;

    gtk_set_locale();
    gtk_init(&argc, &argv);
    if (!(smyrnaGlade))
	smyrnaGlade = smyrnaPath("smyrna.glade");
    xml = glade_xml_new(smyrnaGlade, NULL, NULL);

    gladewidget = glade_xml_get_widget(xml, "frmMain");
    gtk_widget_show(gladewidget);
    g_signal_connect((gpointer) gladewidget, "destroy",
		     G_CALLBACK(mQuitSlot), NULL);
    glade_xml_signal_autoconnect(xml);
    gtk_gl_init(0, 0);
    /* Configure OpenGL framebuffer. */
    glconfig = configure_gl();
    gladewidget = glade_xml_get_widget(xml, "hbox11");

    gtk_widget_hide(glade_xml_get_widget(xml, "vbox13"));
    gtk_window_set_deletable ((GtkWindow*)glade_xml_get_widget(xml, "dlgSettings"),0);
    gtk_window_set_deletable ((GtkWindow*)glade_xml_get_widget(xml, "frmTVNodes"),0);
    create_window(glconfig, gladewidget);
    change_cursor(GDK_TOP_LEFT_ARROW);

#ifndef _WIN32
    glutInit(&argc, argv);
#endif

    gladewidget = glade_xml_get_widget(xml, "hbox13");
    graphComboBox = (GtkComboBox *) gtk_combo_box_new_text();
    gtk_box_pack_end((GtkBox*)gladewidget, (GtkWidget*)graphComboBox, 1, 1, 10);
    gtk_widget_show((GtkWidget*)graphComboBox);
    view->graphComboBox = graphComboBox;

    if(view->guiMode!=GUI_FULLSCREEN)
	gtk_main();
}




int main(int argc, char *argv[])
{
    smyrnaDir = getenv("SMYRNA_PATH");
    if (!smyrnaDir) {
#ifdef _WIN32
#define BSZ 1024
#define SMYRNA "\\share\\graphviz\\smyrna"

	int r;
	char line[BSZ];
	char* s;
	MEMORY_BASIC_INFORMATION mbi;

	if (VirtualQuery (&main, &mbi, sizeof(mbi)) == 0) {
	    fprintf (stderr,"failed to get handle for executable.\n");
	    return 1;
	}
	r = GetModuleFileName ((HMODULE)mbi.AllocationBase, line, BSZ);
	if (!r || (r == BSZ)) {
	    fprintf (stderr,"failed to get path for executable.\n");
	    return 1;
	}

	/* line contains path to smyrna: "$GVROOT\bin\smyrna.exe" 
	 * We want to obtain $GVROOT. We find the second rightmost '\'
	 * and store a '\0' there.
 	 */
	s = strrchr(line,'\\');
	if (!s) {
	    fprintf (stderr,"no backslash in path %s.\n", line);
	    return 1;
	}
	while ((s != line) && (*(--s) != '\\')) ;
	if (s == line) {
	    fprintf (stderr,"no backslash in path %s.\n", line);
	    return 1;
	}
	*s = '\0';

	smyrnaDir = N_NEW(strlen(line)+sizeof(SMYRNA), char);
	strcpy (smyrnaDir, line);
	strcat(smyrnaDir, SMYRNA);
#else
	smyrnaDir = SMYRNA_PATH;
#endif
    }
    load_attributes();

#ifdef G_OS_WIN32
    package_prefix =
	g_win32_get_package_installation_directory(NULL, NULL);
    package_data_dir = g_build_filename(package_prefix, "share", NULL);
    package_locale_dir =
	g_build_filename(package_prefix, "share", "locale", NULL);
#else
    package_locale_dir = g_build_filename(smyrnaDir, "locale", NULL);
#endif				/* # */
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, package_locale_dir);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    view = NEW(ViewInfo);
    init_viewport(view);
    view->initFileName = parseArgs(argc, argv, view);
    if(view->initFileName)
	view->initFile=1;

    if(view->guiMode==GUI_FULLSCREEN)
	cb_glutinit(0,0,800,600,32,75,1,&argc,argv,view->optArg);
    else
	windowedMode(argc, argv);
#ifdef G_OS_WIN32
    g_free(package_prefix);
    g_free(package_data_dir);
#endif
    g_free(package_locale_dir);
    return 0;
}
