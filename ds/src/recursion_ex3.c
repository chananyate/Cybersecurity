#include <stdio.h>
#include "stack.h"
#include "recursion_ex3.h"


/*inserts a new value into a stack that's already sorted in ascending order*/
void InsertSorted(stack_t *stack, int value)
{
	int tmp;

	/*base case. If the stack is empty or if the value is greater than the top element of the stack 
	  then that value gets pushed onto the stack.*/
	if(StackIsEmpty(stack) || value > *(int*)StackPeek(stack))
	{
		StackPush(stack, &value);

		return;
	}

	tmp = *(int*)StackPeek(stack);

	/*pops the top element off*/
	StackPop(stack);

	/*recursive call where the condition is rechecked with the new top-of-stack*/
	InsertSorted(stack, value);

	/*after base case is reached and 'value' is pushed onto the stack, 
	  the recursive call returns and 'tmp' gets pushed back onto the stack.*/
	StackPush(stack, &tmp);

}

/*takes an unsorted stack and sorts it in ascending order*/
void SortStack(stack_t *stack)
{
	int tmp;

	/*base case*/
	if(StackIsEmpty(stack))
		return;
	
	tmp = *(int*)StackPeek(stack);

	StackPop(stack);

	/*recursive call until stack is empty*/
	SortStack(stack);

	/*inserts the popped element (tmp) back into the correct position in the stack*/
	InsertSorted(stack, tmp);

}

void PrintStack(stack_t *stack)
{
	int value;

	/*creates a temp stack to print*/
	stack_t *tmp_stack = StackCreate(sizeof(int), StackCapacity(stack));

	while (!StackIsEmpty(stack))
	{
		value = *(int*)StackPeek(stack);

		printf("%d\n", value);

	    StackPop(stack);

	    /*pushes the popped off value from the original stack onto the temp stack*/
	    StackPush(tmp_stack, &value);
	}

	/*pops off the temp stack and pushes them back onto the original stack*/
	while (!StackIsEmpty(tmp_stack))
	{
		value = *(int*)StackPeek(tmp_stack);

		StackPop(tmp_stack);

	    StackPush(stack, &value);
	}
	printf("\n");

	StackDestroy(tmp_stack);
}