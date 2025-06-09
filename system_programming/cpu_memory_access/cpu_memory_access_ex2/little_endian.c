#include <stdio.h>

int Little_Endian(unsigned int number);

int main()
{
	unsigned int number;
	int result;

	number = 1;

	result = Little_Endian(number);

	if (result == 1)
		printf("The system is little endian.\n");

	else
		printf("The system is not little endian\n");

	return 0;
}


int Little_Endian(unsigned int number)
{
   char *c = (char*)&number;/*casts the pointer from unsigned int* to char* in order to interpret the memory address of "number" as a series of individual bytes*/

   if (*c)/*dereferencign c to get the first byte of "number". In little-endian systems, the least significant byte (which is 1 in this case) is stored at the lowest memory address.*/    
       return 1;

   else
       return 0;
}
