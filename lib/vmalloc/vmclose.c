/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <vmalloc/vmalloc.h>
#include <stdlib.h>

/** Close down a region.
 *
 * @param vm Vmalloc to operate on
 * @returns 0 on success
 */
int vmclose(Vmalloc_t *vm) {

  // clear the region
  int r = vmclear(vm);
  if (r != 0) {
    return r;
  }

  // free the allocator itself
  free(vm);

  return 0;
}
