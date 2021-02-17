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

/*	Put out a null-terminated string
**
**	Written by Kiem-Phong Vo.
*/
/**
 * @param f write to this stream
 * @param s string to write
 * @param rc record separator
 */
ssize_t sfputr(Sfio_t * f, const char *s, int rc)
{
    ssize_t p, n, w;
    uchar *ps;

    SFMTXSTART(f, -1);

    if (f->mode != SF_WRITE && _sfmode(f, SF_WRITE, 0) < 0)
	SFMTXRETURN(f, -1);

    SFLOCK(f, 0);

    for (w = 0; (*s || rc >= 0);) {
	SFWPEEK(f, ps, p);

	if (p == 0 || (f->flags & SF_WHOLE)) {
	    n = strlen(s);
	    if (p >= (n + (rc < 0 ? 0 : 1))) {	/* buffer can hold everything */
		if (n > 0) {
		    memcpy(ps, s, n);
		    ps += n;
		    w += n;
		}
		if (rc >= 0) {
		    *ps++ = rc;
		    w += 1;
		}
		f->next = ps;
	    } else {		/* create a reserve buffer to hold data */
		Sfrsrv_t *rsrv;

		p = n + (rc >= 0 ? 1 : 0);
		if (!(rsrv = _sfrsrv(f, p)))
		    n = 0;
		else {
		    if (n > 0)
			memcpy(rsrv->data, s, n);
		    if (rc >= 0)
			rsrv->data[n] = rc;
		    if ((n = SFWRITE(f, rsrv->data, p)) < 0)
			n = 0;
		}

		w += n;
	    }
	    break;
	}

	if (*s == 0) {
	    *ps++ = rc;
	    f->next = ps;
	    w += 1;
	    break;
	}
	if ((ps = (uchar *) memccpy(ps, s, '\0', p)) != NULL)
	    ps -= 1;
	else
	    ps = f->next + p;
	s += ps - f->next;
	w += ps - f->next;
	f->next = ps;
    }

    /* sync unseekable shared streams */
    if (f->extent < 0 && (f->flags & SF_SHARE))
	(void) SFFLSBUF(f, -1);

    /* check for line buffering */
    else if ((f->flags & SF_LINE) && !(f->flags & SF_STRING)
	     && (n = f->next - f->data) > 0) {
	if (n > w)
	    n = w;
	f->next -= n;
	(void) SFWRITE(f, (void *) f->next, n);
    }

    SFOPEN(f, 0);
    SFMTXRETURN(f, w);
}
