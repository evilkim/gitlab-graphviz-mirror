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

#include <stdlib.h>
struct vector_struct {
  int maxlen;
  int len;
  void *v;
  size_t size_of_elem;
  void (*deallocator)(void *v);
};

typedef struct vector_struct *Vector;

/* deallocator works on each element of the vector */
Vector Vector_new(int maxlen, size_t size_of_elem, void (*deallocator)(void *v));

Vector Vector_add(Vector v, void *stuff);

Vector Vector_reset(Vector v, void *stuff, int i);

void Vector_delete(Vector v);

void* Vector_get(Vector v, int i);

int Vector_get_length(Vector v);

Vector Vector_reset(Vector v, void *stuff, int i);
