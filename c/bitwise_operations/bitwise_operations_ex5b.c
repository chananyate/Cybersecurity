#include <stdio.h>
#include <stdint.h>


uint8_t ByteMirror(uint8_t n);

int main()
{
	uint8_t number, mirror_number;

	printf("Enter a number: ");
	scanf("%hhu", &number);

	mirror_number = ByteMirror(number);

	printf("The mirror number is: %hhu\n", mirror_number);

	return 0;
}

uint8_t ByteMirror(uint8_t n)
{
	uint8_t number;

	number = n;

	number = ((number & 0xf0) >> 4) | ((number & 0x0f) << 4);/*First extracts the higher nibble by masksing the lower one and then shifts it to the right. Then it does the same for the lower nibble and shifts it to the left. The | operator keeps the two nibbles after the swap.*/ 

	number = ((number & 0xcc) >> 2) | ((number & 0x33) << 2);/*The same only with alternate pairs*/

	number = ((number & 0xaa) >> 1) | ((number & 0x55) << 1);/*Now with alternate single bits*/ 

    return number;
}