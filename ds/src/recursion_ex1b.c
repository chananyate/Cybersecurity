#include <stdio.h>

int Fibonacci(int element_index);

int main ()
{
	int index;
	printf("Enter index: ");
	scanf("%d", &index);

	printf("The number at index %d in the fibonacci sequence is: %d\n", index, Fibonacci(index));

	return 0;
}


int Fibonacci(int element_index)
{
	/*base case of 0 or 1*/
	if (element_index <= 1)
		return element_index;

	return Fibonacci(element_index - 1) + Fibonacci(element_index - 2);
}

