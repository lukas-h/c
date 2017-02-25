#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "lil.h"



struct lil_node *lil_get_entry(Lil *lil, int pos)
{
    struct lil_node *current;
    
    if(pos == 0)
	return NULL;
    else if(pos < 0)
	current = lil->last;
    else if(pos > 0)
	current = lil->first;
    
    while(!(pos == -1 || pos == 1))
    {
	if(pos < 0)
	{
	    if(current->prev != NULL)
		current = current->prev;
	    pos++;
	}
	else if(pos > 0)
	{
	    if(current->next != NULL)
		current = current->next;
	    pos--;
	}
    }

    return current;
}


void lil_insert_after(Lil *head, struct lil_node *elem, struct lil_node *val_node)
{
    struct lil_node *next;
    
    assert(elem != NULL);

    next = elem->next;
    if(next != NULL)
    {
	// We are inserting between two elements
	val_node->next = next;
	next->prev = val_node;
    }
    else
    {
	// We are appending to the list
	val_node->next = NULL;
	head->last = val_node;
    }
    val_node->prev = elem;
    elem->next = val_node;
}


void lil_insert_before(Lil *head, struct lil_node *elem, struct lil_node *val_node)
{
    struct lil_node *prev;
    
    assert(elem != NULL);

    prev = elem->prev;
    if(prev != NULL)
    {
	// We are inserting between two elements
	val_node->prev = prev;
	prev->next = val_node;
    }
    else
    {
	// We are inserting before the first element
	val_node->prev = NULL;
	head->first = val_node;
    }
    val_node->next = elem;
    elem->prev = val_node;
}


Lil *LIL_NEW()
{
    return (Lil*) malloc(sizeof(Lil));
}


void LIL_INSERT(Lil *lil, int pos, void *value)
{
    struct lil_node *val_node, *closest;

    // Create a new node for the value
    val_node = malloc(sizeof(struct lil_node));
    val_node->value = value;

    // Look for the specified element to insert at
    closest = lil_get_entry(lil, pos);

    if(closest == NULL)
    {
	// No element found, val_node will be the only element in the list
	val_node->prev = NULL;
	val_node->next = NULL;
	assert(lil->first == NULL);
	lil->first = val_node;
	assert(lil->last == NULL);
	lil->last = val_node;
    }
    else if(pos < 0)
	lil_insert_after(lil, closest, val_node);
    else
	lil_insert_before(lil, closest, val_node);
}


void *LIL_GET(Lil *lil, int pos)
{
    struct lil_node *node;

    node = lil_get_entry(lil, pos);
    if(node == NULL)
	return NULL;
    return node->value;
}


void LIL_SET(Lil *lil, int pos, void *value)
{
    struct lil_node *node;

    node = lil_get_entry(lil, pos);
    if(node == NULL) 	// This should be an error!
	return;
    node->value = value;
}


int LIL_LENGTH(Lil *lil)
{
    struct lil_node *nodeptr;
    int counter;

    counter = 0;
    for(nodeptr = lil->first; nodeptr != NULL; nodeptr = nodeptr->next)
	counter++;
    return counter;
}


void *LIL_REMOVE(Lil *lil, int pos)
{
    struct lil_node *node, *prev, *next;
    void *value;
    
    node = lil_get_entry(lil, pos);
    if(node == NULL)	// This should be an error!
	return NULL;
    prev = node->prev;
    next = node->next;

    if(prev == NULL)
    {
	// We are deleting the first element of the list
	lil->first = next;
    }
    else
    {
	prev->next = next;
    }
    if(next == NULL)
    {
	// We are deleting the last element of the list
	lil->last = prev;
    }
    else
    {
	next->prev = prev;
    }

    value = node->value;
    free(node);
    return value;
}


void LIL_CLEAR(Lil *lil, void (*freefun)(void*))
{
    struct lil_node *nodeptr, *next;

    for(nodeptr = lil->first; nodeptr != NULL; nodeptr = next)
    {
	next = nodeptr->next;
	if(freefun != NULL)
	    freefun(nodeptr);
    }
    lil->first = NULL;
    lil->last = NULL;
}


void LIL_DESTROY(Lil *lil)
{
    if(lil->first != NULL)
    {
	fprintf(stderr, "\n- LIL Error: You have tried to delete a populated list,\n");
	fprintf(stderr, "^ LIL Error: which is a resource leak!\n");
	fprintf(stderr, "^ LIL Error: Please use LIL_CLEAR(the_list, free) to clear the list.\n");
	fprintf(stderr, "^ LIL Error: List ID: %p\n", lil);
    }
    free(lil);
}


void LIL_ITERATOR_INIT(lil_iterator_t *iter, Lil *lil)
{
    *iter = lil->first;
}

void LIL_TAIL_ITERATOR_INIT(lil_iterator_t *iter, Lil *lil)
{
    *iter = lil->last;
}

bool LIL_ITERATOR_EMPTY(lil_iterator_t *iter)
{
    return *iter == NULL;
}

void LIL_ITERATOR_NEXT(lil_iterator_t *iter)
{
    *iter = (*iter)->next;
}

void LIL_ITERATOR_PREV(lil_iterator_t *iter)
{
    *iter = (*iter)->prev;
}

void *LIL_ITERATOR_VALUE(lil_iterator_t *iter)
{
    return (*iter)->value;
}
