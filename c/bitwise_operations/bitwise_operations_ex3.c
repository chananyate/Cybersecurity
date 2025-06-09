#include <stdio.h>

int add1(int *number);

int main()
{
	int n, result;

	printf("Enter number: ");
	scanf("%d", &n);

	result = (add1(&n));

	printf("The number + 1 is: %d\n", result);

	return 0;
}

int add1(int *number)
{	
	int bit;

	bit = 1;

	while (*number & bit)
	{
		*number = *number ^ bit;

		bit <<= 1;
	}

	*number = *number ^ bit;

	return *number;
}
