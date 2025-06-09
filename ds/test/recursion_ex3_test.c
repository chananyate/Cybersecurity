#include <stdio.h>
#include "stack.h"
#include "recursion_ex3.h"

int main()
{
	int i;

	stack_t *stack = StackCreate(sizeof(int), 10);

	int values[] = {5,8,9,3,2,6,1,10,4,7};

	for (i = 0; i < 10; ++i)
		StackPush(stack, &values[i]);

	printf("Original stack:\n");

	PrintStack(stack);

	SortStack(stack);

	printf("Sorted stack:\n");

	PrintStack(stack);

	StackDestroy(stack);

	return 0;
}