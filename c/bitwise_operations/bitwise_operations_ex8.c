#include <stdio.h>

void Swap(int *a, int *b);

int main()
{
	int a, b;

	printf("Enter two numbers: ");
	scanf("%d %d", &a, &b);

	Swap(&a, &b);

	return 0;
}

void Swap(int *a, int *b)
{
	*a = *a ^ *b;

	*b = *a ^ *b;

	*a = *a ^ *b;

	printf("The swapped numbers are: %d and %d\n", *a, *b);
}