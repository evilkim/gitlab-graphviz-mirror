/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <sparse/general.h>
#include <errno.h>

#ifdef DEBUG
double _statistics[10];
#endif

real drand(){
  return rand()/(real) RAND_MAX;
}

int irand(int n){
  /* 0, 1, ..., n-1 */
  assert(n > 1);
  /*return (int) MIN(floor(drand()*n),n-1);*/
  return rand()%n;
}

int *random_permutation(int n){
  int *p;
  int i, j, pp, len;
  if (n <= 0) return NULL;
  p = MALLOC(sizeof(int)*n);
  for (i = 0; i < n; i++) p[i] = i;

  len = n;
  while (len > 1){
    j = irand(len);
    pp = p[len-1];
    p[len-1] = p[j];
    p[j] = pp;
    len--;
  }
  return p;
}


real* vector_subtract_to(int n, real *x, real *y){
  /* y = x-y */
  int i;
  for (i = 0; i < n; i++) y[i] = x[i] - y[i];
  return y;
}
real vector_product(int n, real *x, real *y){
  real res = 0;
  int i;
  for (i = 0; i < n; i++) res += x[i]*y[i];
  return res;
}

real* vector_saxpy(int n, real *x, real *y, real beta){
  /* y = x+beta*y */
  int i;
  for (i = 0; i < n; i++) y[i] = x[i] + beta*y[i];
  return y;
}

real* vector_saxpy2(int n, real *x, real *y, real beta){
  /* x = x+beta*y */
  int i;
  for (i = 0; i < n; i++) x[i] = x[i] + beta*y[i];
  return x;
}

void vector_print(char *s, int n, real *x){
  int i;
    printf("%s{",s); 
    for (i = 0; i < n; i++) {
      if (i > 0) printf(",");
      printf("%f",x[i]); 
    }
    printf("}\n");
}

void vector_float_take(int n, float *v, int m, int *p, float **u){
  /* take m elements v[p[i]]],i=1,...,m and oput in u */
  int i;

  if (!*u) *u = MALLOC(sizeof(float)*m);

  for (i = 0; i < m; i++) {
    assert(p[i] < n && p[i] >= 0);
    (*u)[i] = v[p[i]];
  }
  
}

static int comp_ascend(const void *s1, const void *s2){
  const real *ss1 = s1;
  const real *ss2 = s2;

  if ((ss1)[0] > (ss2)[0]){
    return 1;
  } else if ((ss1)[0] < (ss2)[0]){
    return -1;
  }
  return 0;
}

static int comp_ascend_int(const void *s1, const void *s2){
  const int *ss1 = s1;
  const int *ss2 = s2;

  if ((ss1)[0] > (ss2)[0]){
    return 1;
  } else if ((ss1)[0] < (ss2)[0]){
    return -1;
  }
  return 0;
}

void vector_ordering(int n, real *v, int **p){
  /* give the position of the smallest, second smallest etc in vector v.
     results in p. If *p == NULL, p is assigned.
  */

  real *u;
  int i;

  if (!*p) *p = MALLOC(sizeof(int)*n);
  u = MALLOC(sizeof(real)*2*n);

  for (i = 0; i < n; i++) {
    u[2*i+1] = i;
    u[2*i] = v[i];
  }

  qsort(u, n, sizeof(real)*2, comp_ascend);

  for (i = 0; i < n; i++) (*p)[i] = (int) u[2*i+1];
  FREE(u);

}

void vector_sort_int(int n, int *v){
  qsort(v, n, sizeof(int), comp_ascend_int);
}

real distance_cropped(real *x, int dim, int i, int j){
  int k;
  real dist = 0.;
  for (k = 0; k < dim; k++) dist += (x[i*dim+k] - x[j*dim + k])*(x[i*dim+k] - x[j*dim + k]);
  dist = sqrt(dist);
  return MAX(dist, MINDIST);
}

real distance(real *x, int dim, int i, int j){
  int k;
  real dist = 0.;
  for (k = 0; k < dim; k++) dist += (x[i*dim+k] - x[j*dim + k])*(x[i*dim+k] - x[j*dim + k]);
  dist = sqrt(dist);
  return dist;
}

real point_distance(real *p1, real *p2, int dim){
  int i;
  real dist;
  dist = 0;
  for (i = 0; i < dim; i++) dist += (p1[i] - p2[i])*(p1[i] - p2[i]);
  return sqrt(dist);
}

char *strip_dir(char *s){
  bool first = true;
  if (!s) return s;
  for (size_t i = strlen(s); ; i--) {
    if (first && s[i] == '.') {/* get rid of .mtx */
      s[i] = '\0';
      first = false;
    }
    if (s[i] == '/') return &s[i+1];
    if (i == 0) {
      break;
    }
  }
  return s;
}

void scale_to_box(real xmin, real ymin, real xmax, real ymax, int n, int dim, real *x){
  real min[3], max[3], min0[3], ratio = 1;
  int i, k;

  for (i = 0; i < dim; i++) {
    min[i] = x[i];
    max[i] = x[i];
  }

  for (i = 0; i < n; i++){
    for (k = 0; k < dim; k++) {
      min[k] = MIN(x[i*dim+k], min[k]);
      max[k] = MAX(x[i*dim+k], max[k]);
    }
  }

  if (max[0] - min[0] != 0) {
    ratio = (xmax-xmin)/(max[0] - min[0]);
  }
  if (max[1] - min[1] != 0) {
    ratio = MIN(ratio, (ymax-ymin)/(max[1] - min[1]));
  }
  
  min0[0] = xmin;
  min0[1] = ymin;
  min0[2] = 0;
  for (i = 0; i < n; i++){
    for (k = 0; k < dim; k++) {
      x[i*dim+k] = min0[k] + (x[i*dim+k] - min[k])*ratio;
    }
  }
  
  
}
