/*************************************************************************
 * Copyright (c) 2014 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef LAB_GAMUT_H
#define LAB_GAMUT_H

#ifdef __cplusplus
extern "C" {
#endif

/*visual studio*/
#if defined(_WIN32) && !defined(LAB_GAMUT_EXPORTS)
#define extern __declspec(dllimport)
#endif
/*end visual studio*/

/** lookup table for the visible spectrum of the CIELAB color space
 *
 * This table is entries of 4-tuples of the form (L*, a*, b* lower bound,
 * b* upper bound). A plain lookup table and/or the use of structs is avoided to
 * save memory during compilation. Without this, MSVC ~2019 exhausts memory in
 * CI.
 *
 * More information about CIELAB:
 *   https://en.wikipedia.org/wiki/CIELAB_color_space
 */
extern const signed char lab_gamut_data[];
extern int lab_gamut_data_size;

#undef extern

#ifdef __cplusplus
}
#endif

#endif
