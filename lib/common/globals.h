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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __CYGWIN__
#if defined(GVDLL)
#if !defined(_BLD_gvc)
#define DECLSPEC	__declspec(dllimport)
#else
#define DECLSPEC __declspec(dllexport)
#endif
#endif
#endif
/*visual studio*/
#ifdef _WIN32
#ifndef GVC_EXPORTS
#define DECLSPEC __declspec(dllimport)
#endif
#endif
/*end visual studio*/

#ifndef DECLSPEC
#define DECLSPEC /* nothing */
#endif

#ifndef EXTERN
#define EXTERN extern
#endif

    DECLSPEC EXTERN char *Version;
    DECLSPEC EXTERN char **Files;	/* from command line */
    DECLSPEC EXTERN const char **Lib;		/* from command line */
    DECLSPEC EXTERN char *CmdName;
    DECLSPEC EXTERN char *Gvfilepath;  /* Per-process path of files allowed in image attributes (also ps libs) */
    DECLSPEC EXTERN char *Gvimagepath; /* Per-graph path of files allowed in image attributes  (also ps libs) */

    DECLSPEC EXTERN unsigned char Verbose;
    DECLSPEC EXTERN unsigned char Reduce;
    DECLSPEC EXTERN int MemTest;
    DECLSPEC EXTERN char *HTTPServerEnVar;
    DECLSPEC EXTERN int graphviz_errors;
    DECLSPEC EXTERN int Nop;
    DECLSPEC EXTERN double PSinputscale;
    DECLSPEC EXTERN int Show_cnt;
    DECLSPEC EXTERN char** Show_boxes;	/* emit code for correct box coordinates */
    DECLSPEC EXTERN int CL_type;		/* NONE, LOCAL, GLOBAL */
    DECLSPEC EXTERN unsigned char Concentrate;	/* if parallel edges should be merged */
    DECLSPEC EXTERN double Epsilon;	/* defined in input_graph */
    DECLSPEC EXTERN int MaxIter;
    DECLSPEC EXTERN int Ndim;
    DECLSPEC EXTERN int State;		/* last finished phase */
    DECLSPEC EXTERN int EdgeLabelsDone;	/* true if edge labels have been positioned */
    DECLSPEC EXTERN double Initial_dist;
    DECLSPEC EXTERN double Damping;
    DECLSPEC EXTERN int Y_invert;	/* invert y in dot & plain output */
    DECLSPEC EXTERN int GvExitOnUsage;   /* gvParseArgs() should exit on usage or error */

    DECLSPEC EXTERN Agsym_t
	*G_activepencolor, *G_activefillcolor,
	*G_visitedpencolor, *G_visitedfillcolor,
	*G_deletedpencolor, *G_deletedfillcolor,
	*G_ordering, *G_peripheries, *G_penwidth,
	*G_gradientangle, *G_margin;
    DECLSPEC EXTERN Agsym_t
	*N_height, *N_width, *N_shape, *N_color, *N_fillcolor,
	*N_activepencolor, *N_activefillcolor,
	*N_selectedpencolor, *N_selectedfillcolor,
	*N_visitedpencolor, *N_visitedfillcolor,
	*N_deletedpencolor, *N_deletedfillcolor,
	*N_fontsize, *N_fontname, *N_fontcolor, *N_margin,
	*N_label, *N_xlabel, *N_nojustify, *N_style, *N_showboxes,
	*N_sides, *N_peripheries, *N_ordering, *N_orientation,
	*N_skew, *N_distortion, *N_fixed, *N_imagescale, *N_imagepos, *N_layer,
	*N_group, *N_comment, *N_vertices, *N_z,
	*N_penwidth, *N_gradientangle;
    DECLSPEC EXTERN Agsym_t
	*E_weight, *E_minlen, *E_color, *E_fillcolor,
	*E_activepencolor, *E_activefillcolor,
	*E_selectedpencolor, *E_selectedfillcolor,
	*E_visitedpencolor, *E_visitedfillcolor,
	*E_deletedpencolor, *E_deletedfillcolor,
	*E_fontsize, *E_fontname, *E_fontcolor,
	*E_label, *E_xlabel, *E_dir, *E_style, *E_decorate,
	*E_showboxes, *E_arrowsz, *E_constr, *E_layer,
	*E_comment, *E_label_float,
	*E_samehead, *E_sametail,
	*E_arrowhead, *E_arrowtail,
	*E_headlabel, *E_taillabel,
	*E_labelfontsize, *E_labelfontname, *E_labelfontcolor,
	*E_labeldistance, *E_labelangle,
	*E_tailclip, *E_headclip,
	*E_penwidth;

    DECLSPEC extern struct fdpParms_s* fdp_parms;

#undef EXTERN
#undef DECLSPEC

#ifdef __cplusplus
}
#endif
