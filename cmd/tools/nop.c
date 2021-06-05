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

#include <cgraph/cgraph.h>
#include <ingraphs/ingraphs.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <getopt.h>

static char **Files;
static bool chkOnly;

static const char useString[] = "Usage: nop [-p?] <files>\n\
  -p - check for valid DOT\n\
  -? - print usage\n\
If no files are specified, stdin is used\n";

static void usage(int v)
{
    printf("%s",useString);
    exit(v);
}

static void init(int argc, char *argv[])
{
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "p?")) != -1) {
	switch (c) {
	case 'p':
	    chkOnly = true;
	    break;
	case '?':
	    if (optopt == '\0' || optopt == '?')
		usage(EXIT_SUCCESS);
	    else {
		fprintf(stderr, "nop: option -%c unrecognized\n",
			optopt);
		usage(EXIT_FAILURE);
	    }
	    break;
	default:
	    fprintf(stderr, "nop: unexpected error\n");
	    exit(EXIT_FAILURE);
	}
    }
    argv += optind;
    argc -= optind;

    if (argc)
	Files = argv;
}

static Agraph_t *gread(FILE * fp)
{
    return agread(fp, NULL);
}

int main(int argc, char **argv)
{
    Agraph_t *g;
    ingraph_state ig;

    init(argc, argv);
    newIngraph(&ig, Files, gread);

    while ((g = nextGraph(&ig)) != 0) {
	if (!chkOnly) agwrite(g, stdout);
	agclose(g);
    }

    return (ig.errors != 0 || agerrors() != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
