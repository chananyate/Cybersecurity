#include <stdio.h>

int BothOn(unsigned char number);

int main()
{
	unsigned char number;
	int result;
	result = 0;

	printf("Enter number: ");
	scanf("%u",(unsigned int *)&number);

	result = BothOn(number);

	if (result == 1)
		printf("Both digits 2 and 6 are on\n");
	else
		printf("Both digits are not on\n");

	return 0;
}

int BothOn(unsigned char number)
{
	int bits;

	bits = 0;

	bits = (number & 0x02) && (number & 0x20);

	return bits;
}