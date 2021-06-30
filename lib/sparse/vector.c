/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <sparse/general.h>
#include <sparse/vector.h>


/*---------------- base vector class ----------- */
Vector Vector_new(int maxlen, size_t size_of_elem, void (*deallocator)(void *v)){
  Vector v;
  v = malloc(sizeof(struct vector_struct));
  if (v == NULL)
    return NULL;
  if (maxlen <= 0) maxlen = 1;
  v->maxlen = maxlen;
  v->len = 0;
  v->size_of_elem = size_of_elem;
  v->deallocator = deallocator;
  v->v = malloc(size_of_elem*maxlen);
  if (!v->v){
    free(v);
    return NULL;
  }
  return v;
}

static Vector Vector_assign(Vector v, void *stuff, int i){
  memcpy(((char*) v->v)+(v->size_of_elem)*i/sizeof(char), stuff, v->size_of_elem);
  return v;
}

Vector Vector_reset(Vector v, void *stuff, int i){
  if (i >= v->len) return NULL;
  if (v->deallocator)(v->deallocator)((char*)v->v + (v->size_of_elem)*i/sizeof(char)); 
  return Vector_assign(v, stuff, i);
}


Vector Vector_add(Vector v, void *stuff){
  if (v->len + 1 >= v->maxlen){
    v->maxlen = v->maxlen + MAX((int) .2*(v->maxlen), 10);
    v->v = realloc(v->v, (v->maxlen)*(v->size_of_elem));
    if (!(v->v)) return NULL;
  }

  return Vector_assign(v, stuff, (v->len)++);
}

void Vector_delete(Vector v){
  int i;
  if (!v) return;
  for (i = 0; i < v->len; i++){
    if (v->deallocator)(v->deallocator)((char*)v->v + (v->size_of_elem)*i/sizeof(char));
  }
  free(v->v);
  v->v = NULL;
  free(v);
};

void* Vector_get(Vector v, int i){
  if (i >= v->len) return NULL;
  return ((char*)v->v + i*(v->size_of_elem)/sizeof(char));
}

int Vector_get_length(Vector v){
  return v->len;
}
