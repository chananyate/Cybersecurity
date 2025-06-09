#include <stdio.h>

int AtLeastOne(unsigned char number);

int main()
{
	unsigned char number;
	int result;
	result = 0;

	printf("Enter number: ");
	scanf("%u",(unsigned int *)&number);

	result = AtLeastOne(number);

	if (result != 0)
		printf("At least one of the digits is on\n");
	else
		printf("Neither of the digits is on\n");

	return 0;
}

int AtLeastOne(unsigned char number)
{
	int bits;

	bits = 0;

	bits = number & 0x22;

	return bits;
}