#include <stdio.h>
#include <stdlib.h>

int main()
{

static int s_i = 7;
int i = 7;
int *ptr = &i;
int *ptr2 = (int *)malloc(sizeof(int));//Even though the pointer is assigned (given) an address on the heap via malloc, the pointer's own address is on the stack.
static int **ptr3;

if(ptr)
{
    int **ptr3 = &ptr;
}
printf("The address of s_i is %p:\n", &s_i);
printf("The address of i is %p:\n", &i);
printf("The address of ptr is %p:\n", &ptr);
printf("The address of ptr2 is %p:\n", &ptr2);
printf("The address of ptr3 is %p:\n", &ptr3);



free(ptr2);


}
