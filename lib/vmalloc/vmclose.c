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

/** Close down a region.
 *
 * @param vm Vmalloc to operate on
 * @returns 0 on success
 */
int vmclose(Vmalloc_t *vm) {
  int r;

  /* clear the region */
  r = vmclear(vm);
  if (r != 0) {
    return r;
  }

  /* free the allocator itself */
  free(vm);

  return 0;
}
