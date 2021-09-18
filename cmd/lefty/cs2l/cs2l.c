/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/


#include <ast.h>
#include <cs.h>
#include <msg.h>
#include "common.h"
#include "code.h"
#include "mem.h"
#include "tbl.h"
#include "exec.h"
#include "cs2l.h"

int C2Lopen (char *name, char *mode, FILE **ifp, FILE **ofp) {
    int fd;

    if ((fd = csopen (name, CS_OPEN_READ)) == -1)
        return -1;
    fcntl (fd, F_SETFD, FD_CLOEXEC);
    *ifp = fdopen (fd, "r"), *ofp = fdopen (fd, "a+");
    return 0;
}

/* LEFTY builtin */
int C2Lreadcsmessage (int argc, lvar_t *argv) {
    return L_SUCCESS;
}
