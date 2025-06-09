#define SIZEOF_VAR(a) ((char*)(&a + 1) - (char*)(&a))/*pointer arithmetics. Casting to char is done to get sizes in single bytes*/ 
#include <stdio.h>

int main()
{
	int a, size;

	printf("Enter number: ");
	scanf("%d", &a);

	size = SIZEOF_VAR(a);

	printf("The size is: %d\n", size);

	return 0;
}