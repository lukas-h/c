#include <string.h>
#include <stdlib.h>

#include "stack.h"

const stack_item_t nil_template = {"\0", NIL, {0.0, NULL}};

my_stack_t *stack_create(){
	my_stack_t *tmp;
	tmp = malloc(sizeof(my_stack_t));
	tmp->itemv = (stack_item_t *)malloc(sizeof(stack_item_t));
	tmp->itemc=0;
	return tmp;
}

void stack_destroy(my_stack_t *stack){
	if(stack==NULL){
		error("could not destroy stack\n");
		return;
	}
	if(stack->itemc > 0){
		free(stack->itemv);
	}
	free(stack);
}

void stack_push(my_stack_t *stack, stack_item_t item){
	stack->itemv[stack->itemc] = item;
	stack->itemc++;
	stack->itemv = (stack_item_t *)realloc(stack->itemv, sizeof(stack_item_t) * (stack->itemc+1));
}

stack_item_t stack_pop(my_stack_t *stack){
	stack_item_t tmp;
	if(stack->itemc==0){
		error("could not pop value from empty stack\n");
		return nil_template;
	}
	stack->itemc--;
	tmp = stack->itemv[stack->itemc];
	stack->itemv = (stack_item_t *)realloc(stack->itemv, sizeof(stack_item_t) * (stack->itemc+1));
	
	return tmp;
}

size_t stack_get_pos_by_name(my_stack_t *stack, char *str){
	size_t ret;
	for(ret=0; ret < stack->itemc; ret++){
		if(strcmp(str, stack->itemv[ret].name)==0){
			return ret;
		}
	}
	error("could not find name in stack\n");
	return -1;
}

stack_item_t stack_get_item(my_stack_t *stack, size_t pos){
	if(pos >= stack->itemc){
		error("could not get item\n");
		return nil_template;
	}
	return stack->itemv[pos];
}

void stack_set_item(my_stack_t *stack, size_t pos, stack_item_t item){
	if(pos >= stack->itemc){
		error("could not set item\n");
		return;
	}
	stack->itemv[pos] = item;
}

#ifdef DEBUG
int main(){
	my_stack_t *stack = stack_create();

	stack_item_t item, item2, item3;
	strcpy(item.name, "foo");
	item.type = NUMERIC;
	item.val.num = 3.14159;

	strcpy(item3.name, "bar");
	item3.type = NUMERIC;
	item.val.num = 2.71828;

	stack_push(stack, item);

	item2 = stack_pop(stack);
	item = stack_pop(stack);

	stack_push(stack, item2);
	stack_set_item(stack, stack_get_pos_by_name(stack, "foo"), item3);
	item = stack_get_item(stack, stack_get_pos_by_name(stack, "bar"));

	stack_destroy(stack);
	return 0;
}
#endif