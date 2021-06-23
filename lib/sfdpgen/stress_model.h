#pragma once

void stress_model(int dim, SparseMatrix A, SparseMatrix D, real **x, int edge_len_weighted, int maxit, real tol, int *flag);
