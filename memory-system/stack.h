#ifndef _STACK_H
#define _STACK_H

#include <stdio.h> // for size_t

#define error(msg) fprintf(stderr, "error: " msg)

/*
 *  example variable type implementation
 */
typedef struct _stack_item_t stack_item_t;
struct _stack_item_t{
	
#define MAX_LEN_OF_ID 17U
	char name[MAX_LEN_OF_ID];
	
#define NIL		0
#define NUMERIC 1
#define STRING  2
	char type;

	union{
		double num;
		char *str;
	}val;
};


/*
 * the stack type
 */
typedef struct _my_stack_t my_stack_t;
struct _my_stack_t{
	size_t itemc;
	stack_item_t *itemv;
};

/*
 * stack functions
 */
 
extern my_stack_t *stack_create(void);
extern void stack_destroy(my_stack_t *stack);

extern void stack_push(my_stack_t *stack, stack_item_t item);
extern stack_item_t stack_pop(my_stack_t *stack);

extern size_t stack_get_pos_by_name(my_stack_t *stack, char *str);

extern stack_item_t stack_get_item(my_stack_t *stack, size_t pos);
extern void stack_set_item(my_stack_t *stack, size_t pos, stack_item_t val);

#endif
