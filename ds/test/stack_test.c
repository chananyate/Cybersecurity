#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
    int a, b, c, d;/*elements to push onto the stack*/

    stack_t *stack = NULL;

    int *peeked = NULL;

    a = 10;

    b = 20;

    c = 30;
    
    stack = StackCreate(sizeof(int), 3);/*a stack of 3 integers*/
    
    if (!stack)/*prints error message and returns 1 if "stack" pointer is NULL*/
    {
        printf("Failed to create the stack.\n");
        return 1;
    }

    printf("Stack created. Capacity: %lu\n", StackCapacity(stack));
    
    StackPush(stack, &a);

    StackPush(stack, &b);

    StackPush(stack, &c);

    d = 40;/*tests to see what happens if I push past capacity*/

    StackPush(stack, &d); 

    printf("Stack size after pushing 3 elements: %lu\n", StackSize(stack));

    peeked = (int*)StackPeek(stack);/*displays the top element*/

    if (peeked)/*checks to see that the pointer "peeked" is not NULL*/
        printf("Top element: %d\n", *peeked);

    StackPop(stack);

    printf("Stack size after one pop: %lu\n", StackSize(stack));

    peeked = (int*)StackPeek(stack);

    if (peeked)
        printf("Top element after pop: %d\n", *peeked);

    if (StackIsEmpty(stack))
        printf("Stack is empty.\n");

    else
        printf("Stack is not empty.\n");

    StackPop(stack);

    StackPop(stack);

    printf("Stack size after popping all elements: %lu\n", StackSize(stack));/*stack should be empty now*/

    if (StackIsEmpty(stack))
        printf("Stack is empty.\n");

    else
        printf("Stack is not empty.\n");

    StackPop(stack);/*attempts to pop from an empty stack, should print "Nothing to pop off"*/

    StackDestroy(stack);

    printf("Stack destroyed.\n");

    return 0;
}
