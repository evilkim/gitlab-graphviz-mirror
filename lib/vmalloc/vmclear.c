/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: See CVS logs. Details at http://www.graphviz.org/
 *************************************************************************/

#include <vmalloc/vmhdr.h>
#include <vmalloc/vmalloc.h>
#include <stdlib.h>

/** Clear out all allocated space.
 *
 * Note that this leaves the allocation region itself usable, but just frees all
 * previous allocations made within this region.
 *
 * @param vm Vmalloc to operate on
 * @returns 0 on success
 */
int vmclear(Vmalloc_t *vm) {
  size_t i;

  /* free all allocated pointers */
  for (i = 0; i < vm->size; ++i) {
    free(vm->allocated[i]);
  }

  /* reset our metadata */
  free(vm->allocated);
  vm->allocated = NULL;
  vm->size = vm->capacity = 0;

  return 0;
}
