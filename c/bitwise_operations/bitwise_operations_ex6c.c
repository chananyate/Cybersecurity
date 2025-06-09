#include <stdio.h>

void Swap(unsigned char *number);

int main()
{
	unsigned char number;

	printf("Enter number: ");
	scanf("%hhu",&number);

	Swap(&number);

	printf("The number after swapping its third and fifth digit is: %u\n", number);

	return 0;
}

void Swap(unsigned char *number)
{
	unsigned char third_bit = (*number & 0x04);
	unsigned char fifth_bit = (*number & 0x10);

	*number &= ~(0x04 | 0x10);

	*number |= (third_bit << 2) | (fifth_bit >> 2);
}