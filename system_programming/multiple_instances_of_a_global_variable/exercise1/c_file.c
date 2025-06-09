#include <stdio.h>
#include "headers.h"

int x = 10;

void PrintGlobalVariableAdress()
{
	printf("The address of x is %p\n", (void*)&x);
}
