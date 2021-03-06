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

/*	Resize a stream.
	Written by Kiem-Phong Vo.
*/

int sfresize(Sfio_t * f, Sfoff_t size)
{
    SFMTXSTART(f, -1);

    if (size < 0 || f->extent < 0 ||
	(f->mode != SF_WRITE && _sfmode(f, SF_WRITE, 0) < 0))
	SFMTXRETURN(f, -1);

    SFLOCK(f, 0);

    if (f->flags & SF_STRING) {
	SFSTRSIZE(f);

	if (f->extent >= size) {
	    if ((f->flags & SF_MALLOC) && (f->next - f->data) <= size) {
		size_t s = (((size_t) size + 1023) / 1024) * 1024;
		void *d;
		if (s < f->size && (d = realloc(f->data, s))) {
		    f->data = d;
		    f->size = s;
		    f->extent = s;
		}
	    }
	    memclear((char *) (f->data + size), (int) (f->extent - size));
	} else {
	    if (SFSK(f, size, SEEK_SET, f->disc) != size)
		SFMTXRETURN(f, -1);
	    memclear((char *) (f->data + f->extent),
		     (int) (size - f->extent));
	}
    } else {
	if (f->next > f->data)
	    SFSYNC(f);
#ifdef HAVE_FTRUNCATE
	if (ftruncate(f->file, size) < 0)
	    SFMTXRETURN(f, -1);
#else
	SFMTXRETURN(f, -1);
#endif
    }

    f->extent = size;

    SFOPEN(f, 0);

    SFMTXRETURN(f, 0);
}
