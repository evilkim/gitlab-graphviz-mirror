/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

/*
 * Glenn Fowler
 * AT&T Research
 *
 * return lex name for op[subop]
 */

#include <expr/exlib.h>
#include <expr/exop.h>
#include <stddef.h>

#define TOTNAME		3
#define MAXNAME		16

char*
exlexname(int op, int subop)
{
	char*	b;

	static int	n;
	static char	buf[TOTNAME][MAXNAME];

	if (op > MINTOKEN && op < MAXTOKEN)
		return (char*)exop((size_t)op - MINTOKEN);
	if (++n > TOTNAME)
		n = 0;
	b = buf[n];
	if (op == '=')
	{
		if (subop > MINTOKEN && subop < MAXTOKEN)
			sfsprintf(b, MAXNAME, "%s=", exop((size_t)subop - MINTOKEN));
		else if (subop > ' ' && subop <= '~')
			sfsprintf(b, MAXNAME, "%c=", subop);
		else sfsprintf(b, MAXNAME, "(%d)=", subop);
	}
	else if (op > ' ' && op <= '~')
		sfsprintf(b, MAXNAME, "%c", op);
	else sfsprintf(b, MAXNAME, "(%d)", op);
	return b;
}
