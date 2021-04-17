/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include	<sfio/sfhdr.h>
#include	<stddef.h>

/*	External variables and functions used only by Sfio
**	Written by Kiem-Phong Vo
*/

/* global variables used internally to the package */
Sfextern_t _Sfextern;

/* accessible to application code for a few fast macro functions */
ssize_t _Sfi = -1;

#define SFMTXIN		(0)
#define SFMTXOUT	(0)
#define SFMTXERR	(0)

Sfio_t _Sfstdin = SFNEW(NULL, -1, 0, (SF_READ | SF_STATIC | SF_MTSAFE), NULL,
			SFMTXIN);
Sfio_t _Sfstdout = SFNEW(NULL, -1, 1, (SF_WRITE | SF_STATIC | SF_MTSAFE), NULL,
			SFMTXOUT);
Sfio_t _Sfstderr = SFNEW(NULL, -1, 2, (SF_WRITE | SF_STATIC | SF_MTSAFE), NULL,
			SFMTXERR);

Sfio_t *sfstdin = &_Sfstdin;
Sfio_t *sfstdout = &_Sfstdout;
Sfio_t *sfstderr = &_Sfstderr;
