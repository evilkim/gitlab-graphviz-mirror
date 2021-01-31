/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <vmalloc/vmhdr.h>
#include <vmalloc/vmalloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/** make room to store a new pointer we are about to allocate
 *
 * @param vm Vmalloc to operate on
 * @returns true on success
 */
static bool make_space(Vmalloc_t *vm) {

  if (vm->size == vm->capacity) {

    // expand our allocation storage
    size_t c = vm->capacity == 0 ? 1 : vm->capacity * 2;
    void **p = realloc(vm->allocated, sizeof(vm->allocated[0]) * c);
    if (p == NULL) {
      return false;
    }

    // save the new array
    vm->allocated = p;
    vm->capacity = c;
  }

  return true;
}

/** allocate heap memory
 *
 * @param vm region allocating from
 * @param size desired block size
 * @returns Memory fulfilling the allocation request or NULL on failure
 */
void *bestalloc(Vmalloc_t *vm, size_t size) {

  if (!make_space(vm)) {
    return NULL;
  }

  void *p = malloc(size);
  if (p == NULL) {
    return NULL;
  }

  vm->allocated[vm->size] = p;
  ++vm->size;

  return p;
}

/** free heap memory
 *
 * @param vm Region the pointer was originally allocated from
 * @param data The pointer originally received from vmalloc
 * @returns 0 on success
 */
int bestfree(Vmalloc_t *vm, void *data) {

  if (!data) { // ANSI-ism
    return 0;
  }

  // find the pointer we previously allocated
  for (size_t i = 0; i < vm->size; ++i) {
    if (vm->allocated[i] == data) {

      // clear this slot
      size_t extent = sizeof(vm->allocated[0]) * (vm->size - i - 1);
      memmove(vm->allocated + i, vm->allocated + i + 1, extent);
      --vm->size;

      // give this back to the underlying allocator
      free(data);

      return 0;
    }
  }

  // we did not find this pointer; free() of something we did not allocate
  return -1;
}

/** resize an area of allocated memory
 *
 * @param vm region allocation from
 * @param data old block of data
 * @param size new size
 * @returns Pointer to the newly resized area or NULL on failure
 */
void *bestresize(Vmalloc_t *vm, void *data, size_t size) {

  if (!data) {
    return bestalloc(vm, size);
  }

  // find the pointer we previously allocated
  for (size_t i = 0; i < vm->size; ++i) {
    if (vm->allocated[i] == data) {

      // resize the allocation
      void *p = realloc(data, size);
      if (p == NULL) {
        return p;
      }

      // save the updated pointer
      vm->allocated[i] = p;

      return p;
    }
  }

  // the pointer the caller gave us was not allocated by us
  return NULL;
}
