#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"


struct stack 
{
	size_t size;

	size_t capacity;

	size_t element_size;

	void *array;
};

stack_t *StackCreate(size_t element_size, size_t capacity)
{
	stack_t *stack = NULL;

	size_t total_size;

	total_size = element_size * capacity + sizeof(stack_t);

	stack = malloc(total_size);

	assert(NULL != stack);

	stack->array = (void*)((char*)stack + sizeof(stack_t));/*advances the pointer to point just past the stack_t structure*/

	stack->capacity = capacity;

	stack->element_size = element_size;

	stack->size = 0;

	return stack;
}

void StackDestroy(stack_t *stack)
{
	free(stack);
}

void StackPop(stack_t *stack)
{
	if (StackIsEmpty(stack))
	{
		printf("Nothing to pop off.\n");
		return;
	}

	stack->size--;/*the pop function's sole purpose is to update stack->size so that other functions (like push and peek) can perform pointer arithmetics based on it in order to determine the position of elements within the stack.*/
}

void StackPush(stack_t *stack, void *element)
{

	void *target = NULL;
	if (stack->size == StackCapacity(stack))
	{
		printf("Stack is full.\n");
		return;
	}

	target = (char *)stack->array + (stack->size * stack->element_size);/*advancing pointer to point just past the top element*/

    memcpy(target, element, stack->element_size);/*copies the content from "element" to the location pointed to by "target". We use memcpy instead of dereferencing "target" and assigning "element" directly because the stack is generic and uses void pointers. For direct assignment we would need to know the specific data type, whereas memcpy handles the copying for any data type.*/

	stack->size++;
}

void *StackPeek(stack_t *stack)
{
	if (StackIsEmpty(stack))
		return NULL;

	return (char *)stack->array + ((stack->size - 1) * stack->element_size);/*advances pointer to point to the top element*/
}

size_t StackSize(stack_t *stack)
{
	return stack->size;
}

int StackIsEmpty(stack_t *stack)
{
	return stack->size == 0;/*returns 1 if the stack is empty, 0 if not*/
}


size_t StackCapacity(stack_t *stack)
{
	assert(NULL != stack);

	return stack->capacity;
}


