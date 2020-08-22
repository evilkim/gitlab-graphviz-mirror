/* $Id$Revision: */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      See the LICENSE file for copyright information.     *
**********************************************************/

#include <rbtree/red_black_tree.h>

/*  NullFunction does nothing it is included so that it can be passed */
/*  as a function to RBTreeCreate when no other suitable function has */
/*  been defined */

void NullFunction(void * junk) { ; }
