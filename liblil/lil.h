#ifndef LIL_H
#define LIL_H

#include <stdbool.h>


struct lil_node
{
    void *value;
    struct lil_node *prev;
    struct lil_node *next;
};

typedef struct lil_node* lil_iterator_t;


typedef struct lil_head
{
    struct lil_node *first;
    struct lil_node *last;
} Lil;


Lil  *LIL_NEW();
void  LIL_INSERT(Lil*, int, void*);
void *LIL_GET(Lil*, int);
void  LIL_SET(Lil*, int, void*);
int   LIL_LENGTH(Lil*);
void *LIL_REMOVE(Lil*, int);
void  LIL_CLEAR(Lil*, void (*)(void*));
void  LIL_DESTROY(Lil*);

void  LIL_ITERATOR_INIT(lil_iterator_t*, Lil*);
void  LIL_TAIL_ITERATOR_INIT(lil_iterator_t*, Lil*);
bool  LIL_ITERATOR_EMPTY(lil_iterator_t*);
void  LIL_ITERATOR_NEXT(lil_iterator_t*);
void  LIL_ITERATOR_PREV(lil_iterator_t*);
void *LIL_ITERATOR_VALUE(lil_iterator_t*);

#endif
