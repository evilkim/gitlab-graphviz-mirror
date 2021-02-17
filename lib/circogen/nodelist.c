/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/


#include	<circogen/nodelist.h>
#include	<circogen/circular.h>
#include	<assert.h>

static nodelistitem_t *init_nodelistitem(Agnode_t * n)
{
    nodelistitem_t *p = NEW(nodelistitem_t);
    p->curr = n;
    return p;
}

nodelist_t *mkNodelist()
{
    nodelist_t *list = NEW(nodelist_t);
    return list;
}

void freeNodelist(nodelist_t * list)
{
    nodelistitem_t *temp;
    nodelistitem_t *next;

    if (!list)
	return;

    for (temp = list->first; temp; temp = next) {
	next = temp->next;
	free(temp);
    }
    free(list);
}

/* appendNodelist:
 * Add node after one.
 * If one == NULL, add n to end.
 */
void appendNodelist(nodelist_t * list, nodelistitem_t * one, Agnode_t * n)
{
    nodelistitem_t *np = init_nodelistitem(n);

    list->sz++;
    if (!one)
	one = list->last;
    if (one == list->last) {
	if (one)
	    one->next = np;
	else
	    list->first = np;
	np->prev = one;
	np->next = NULL;
	list->last = np;
    } else {
	nodelistitem_t *temp = one->next;
	one->next = np;
	np->prev = one;
	temp->prev = np;
	np->next = temp;
    }
}

/* reverseNodelist;
 * Destructively reverse a list.
 */
nodelist_t *reverseNodelist(nodelist_t * list)
{
    nodelistitem_t *temp;
    nodelistitem_t *p;

    for (p = list->first; p; p = p->prev) {
	temp = p->next;
	p->next = p->prev;
	p->prev = temp;
    }
    temp = list->last;
    list->last = list->first;
    list->first = temp;
    return list;
}

/* realignNodelist:
 * Make np new front of list, with current last hooked to
 * current first. I.e., make list circular, then cut between
 * np and np->prev.
 */
void realignNodelist(nodelist_t * list, nodelistitem_t * np)
{
    nodelistitem_t *temp;
    nodelistitem_t *prev;

    if (np == list->first)
	return;

    temp = list->first;
    prev = np->prev;

    list->first = np;
    np->prev = NULL;
    list->last->next = temp;
    temp->prev = list->last;
    list->last = prev;
    prev->next = NULL;
}

/* cloneNodelist:
 * Create a copy of list.
 */
nodelist_t *cloneNodelist(nodelist_t * list)
{
    nodelist_t *newlist = mkNodelist();
    nodelistitem_t *temp;
    nodelistitem_t *prev = 0;

    for (temp = list->first; temp; temp = temp->next) {
	appendNodelist(newlist, prev, temp->curr);
	prev = newlist->last;
    }
    return newlist;
}

/* insertNodelist:
 * Remove cn. Then, insert cn before neighbor if pos == 0 and 
 * after neighbor otherwise.
 */
void
insertNodelist(nodelist_t * list, Agnode_t * cn, Agnode_t * neighbor,
	       int pos)
{
    nodelistitem_t *temp;
    nodelistitem_t *prev;
    nodelistitem_t *next;
    nodelistitem_t *actual = 0;

    for (temp = list->first; temp; temp = temp->next) {
	if (temp->curr == cn) {
	    actual = temp;
	    prev = actual->prev;
	    next = actual->next;
	    if (prev)		/* not first */
		prev->next = next;
	    else
		list->first = next;

	    if (next)		/* not last */
		next->prev = prev;
	    else
		list->last = prev;
	    break;
	}
    }
    assert(actual);

    prev = NULL;
    for (temp = list->first; temp; temp = temp->next) {
	if (temp->curr == neighbor) {
	    if (pos == 0) {
		if (temp == list->first) {
		    list->first = actual;
		    actual->next = temp;
		    actual->prev = NULL;
		    temp->prev = actual;
		    return;
		}
		prev->next = actual;
		actual->prev = prev;
		actual->next = temp;
		temp->prev = actual;
		return;
	    } else {
		if (temp == list->last) {
		    list->last = actual;
		    actual->next = NULL;
		    actual->prev = temp;
		    temp->next = actual;
		    return;
		}
		actual->prev = temp;
		actual->next = temp->next;
		temp->next->prev = actual;
		temp->next = actual;
		return;
	    }
	    break;
	}
	prev = temp;
    }
}

int sizeNodelist(nodelist_t * list)
{
    return list->sz;
}

/* node_position:
 * Returns index of node n in list, starting at 0.
 * Returns -1 if not in list.
 */
int node_position(nodelist_t * list, Agnode_t * n)
{
    return POSITION(n);
}

/* concatNodelist:
 * attach l2 to l1.
 */
static void concatNodelist(nodelist_t * l1, nodelist_t * l2)
{
    if (l2->first) {
	if (l2->first) {
	    l1->last->next = l2->first;
	    l2->first->prev = l1->last;
	    l1->last = l2->last;
	    l1->sz += l2->sz;
	} else {
	    *l1 = *l2;
	}
    }
}

/* reverse_append;
 * Create l1 @ (rev l2)
 * Destroys and frees l2.
 */
void reverseAppend(nodelist_t * l1, nodelist_t * l2)
{
    l2 = reverseNodelist(l2);
    concatNodelist(l1, l2);
    free(l2);
}

#ifdef DEBUG
void printNodelist(nodelist_t * list)
{
    nodelistitem_t *temp = NULL;

    temp = list->first;
    while (temp != NULL) {
	fprintf(stderr, "%s ", agnameof(temp->curr));
	temp = temp->next;
    }
    fputs("\n", stderr);
}
#endif
