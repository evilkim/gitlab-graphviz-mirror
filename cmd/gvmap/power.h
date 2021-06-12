/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef POWER_H
#define POWER_H

#include <sparse/general.h>

void power_method(void *A, int n, int K, int random_seed, int maxit, real tol,
  real **eigv, real **eigs);

void matvec_sparse(void *M, real *u, real **v, int transposed, int *flag);

#endif
