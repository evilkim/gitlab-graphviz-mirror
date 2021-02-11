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

/*	Public header file for the virtual malloc package.
**
**	Written by Kiem-Phong Vo, kpv@research.att.com, 01/16/94.
*/

    typedef struct _vmalloc_s Vmalloc_t;
    typedef struct _vmethod_s Vmethod_t;

    struct _vmethod_s {
	void *(*allocf) (Vmalloc_t *, size_t);
	void *(*resizef) (Vmalloc_t *, void *, size_t);
	int (*freef) (Vmalloc_t *, void *);
    };

    struct _vmalloc_s {
	Vmethod_t meth;		/* method for allocation        */

	void **allocated;	/* pointers we have given out           */
	size_t size;	/* used entries in `allocated`          */
	size_t capacity;	/* available entries in `allocated`     */
    };

    extern Vmalloc_t *vmopen(void);
    extern int vmclose(Vmalloc_t *);
    extern int vmclear(Vmalloc_t *);

    extern void *vmalloc(Vmalloc_t *, size_t);
    extern void *vmresize(Vmalloc_t *, void *, size_t);
    extern int vmfree(Vmalloc_t *, void *);

    extern long vmaddr(Vmalloc_t *, void *);

    extern char *vmstrdup(Vmalloc_t *, const char *);


/* to coerce any value to a Vmalloc_t*, make ANSI happy */
#define _VM_(vm)	((Vmalloc_t*)(vm))
/* non-debugging/profiling allocation calls */
#ifndef vmalloc
#define vmalloc(vm,sz)		(*(_VM_(vm)->meth.allocf))((vm),(sz))
#endif
#ifndef vmresize
#define vmresize(vm,d,sz)	(*(_VM_(vm)->meth.resizef))\
					((vm),(void*)(d),(sz))
#endif
#ifndef vmfree
#define vmfree(vm,d)		(*(_VM_(vm)->meth.freef))((vm),(void*)(d))
#endif
#define vmnewof(v,p,t,n,x)	vmresize((v), (p), sizeof(t)*(n)+(x))
#ifdef __cplusplus
}
#endif
