#include <stdio.h>

#define LITTLE_ENDIAN(number) \
	(*(unsigned char*)&(number) == (unsigned char)(number))/*casts the address of "number" to a pointer to an unsigned char, thus telling the compiler to interpet the memory as a sequence of bytes. Then dereference it to obtain the value at the first byte (lowest address) and compare it to the LSB of "number" (which is obtained by casting to an unsigned char).*/

int main()
{
	unsigned int number;

	number = 1;

	if (LITTLE_ENDIAN(number))
		printf("The system is little endian.\n");

	else
		printf("The system is not little endian\n");

	return 0;
}