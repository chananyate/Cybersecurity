#include <stdio.h>
#include <stdint.h>


uint8_t ByteMirror(uint8_t number);

int main()
{
	uint8_t number, mirror_number;

	printf("Enter a number: ");
	scanf("%hhu", &number);

	mirror_number = ByteMirror(number);

	printf("The mirror number is: %hhu\n", mirror_number);

	return 0;
}

uint8_t ByteMirror(uint8_t number)
{
	char i; 
	uint8_t reversed;

	reversed = 0;

	for (i = 0; i < 8; ++i)
	{
		reversed <<= 1;

		if (number & 1)/*checks if bit is on*/
			reversed |= 1; /*sets bit*/

     	number >>= 1;
    }

    return reversed;
}