#include <sparse/general.h>
#include <sparse/SparseMatrix.h>
#include <sfdpgen/spring_electrical.h>
#include <sfdpgen/post_process.h>
#include <sfdpgen/stress_model.h>

static void stress_model_core(int dim, SparseMatrix B, real **x, int edge_len_weighted, int maxit_sm, real tol, int *flag){
  int m;
  SparseStressMajorizationSmoother sm;
  real lambda = 0;
  int i;
  SparseMatrix A = B;

  if (!SparseMatrix_is_symmetric(A, FALSE) || A->type != MATRIX_TYPE_REAL){
    if (A->type == MATRIX_TYPE_REAL){
      A = SparseMatrix_symmetrize(A, FALSE);
      A = SparseMatrix_remove_diagonal(A);
    } else {
      A = SparseMatrix_get_real_adjacency_matrix_symmetrized(A);
    } 
  }
  A = SparseMatrix_remove_diagonal(A);

  *flag = 0;
  m = A->m;
  if (!x) {
    *x = MALLOC(sizeof(real)*m*dim);
    srand(123);
    for (i = 0; i < dim*m; i++) (*x)[i] = drand();
  }

  if (edge_len_weighted){
    sm = SparseStressMajorizationSmoother_new(A, dim, lambda, *x, WEIGHTING_SCHEME_SQR_DIST, TRUE);/* do not under weight the long distances */
  } else {
    sm = SparseStressMajorizationSmoother_new(A, dim, lambda, *x, WEIGHTING_SCHEME_NONE, TRUE);/* weight the long distances */
  }

  if (!sm) {
    *flag = -1;
    goto RETURN;
  }


  sm->tol_cg = 0.1; /* we found that there is no need to solve the Laplacian accurately */
  sm->scheme = SM_SCHEME_STRESS;
  SparseStressMajorizationSmoother_smooth(sm, dim, *x, maxit_sm, tol);
  for (i = 0; i < dim*m; i++) {
    (*x)[i] /= sm->scaling;
  }
  SparseStressMajorizationSmoother_delete(sm);

 RETURN:
  if (A != B) SparseMatrix_delete(A);
}

void stress_model(int dim, SparseMatrix A, SparseMatrix D, real **x, int edge_len_weighted, int maxit_sm, real tol, int *flag){
  stress_model_core(dim, D, x, edge_len_weighted, maxit_sm, tol, flag);
}
