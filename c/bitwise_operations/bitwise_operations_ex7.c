#include <stdio.h>

int unsigned RoundDownTo16(unsigned int number);

int main()
{
	unsigned int number, result;

	printf("Enter a number: ");
	scanf("%u", &number);

	result = RoundDownTo16(number);

	if (number < 16)
 	{
 		printf("number not divisible by 16\n");
 		return 1;
 	}

 	else
 	{
 		printf("The nearest smaller number divisible by 16 is: %d\n", result);
	}


	return 0;
}

int unsigned RoundDownTo16(unsigned int number)
{
 	return number & 0xfffffff0;
}