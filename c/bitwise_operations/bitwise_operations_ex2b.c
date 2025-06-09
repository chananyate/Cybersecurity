#include <stdio.h>

int IsPow2(unsigned int number);

int main()
{
	unsigned int number;
	int result;

	result = 0;

	printf("Enter number: ");
	scanf("%u", &number);

    result = IsPow2(number);

    if (result)
    	printf("The number is a power of 2\n");

    else
		printf("The number is not a power of 2\n");

	return 0;
}

int IsPow2(unsigned int number)
{
	unsigned n;
	int result;

	n = 1;

	while (n < number)
	{
		if ((number & n) !=0)
			result = 0;

		else 
			result = 1;

		n <<= 1;
	}

	return result;
}
