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

#include <sparse/SparseMatrix.h>

enum {SOLVE_METHOD_CG, SOLVE_METHOD_JACOBI};

typedef struct Operator_struct *Operator;

struct Operator_struct {
  void *data;
  real* (*Operator_apply)(Operator o, real *in, real *out);
};

real cg(Operator Ax, Operator precond, int n, int dim, real *x0, real *rhs, real tol, int maxit);

real SparseMatrix_solve(SparseMatrix A, int dim, real *x0, real *rhs, real tol, int maxit, int method, int *flag);

Operator Operator_uniform_stress_matmul(SparseMatrix A, real alpha);

Operator Operator_uniform_stress_diag_precon_new(SparseMatrix A, real alpha);
