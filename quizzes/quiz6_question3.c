#include <stdio.h>

int count(long number);

int main ()
{
	long number;

	printf("Enter number: ");
	scanf("%ld", &number);

	printf("The number of set bits is %d\n", count(number));

	return 0;
}


int count(long number)
{
	int count;

	count = 0;
	
	while (number > 0)
	{
		number = number & (number - 1);

		++count;
	}

	return count;
}