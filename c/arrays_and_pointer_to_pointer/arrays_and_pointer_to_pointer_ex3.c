#include <stdio.h>

int main()
{
     
    printf("Size of data types: short int: %lu\n", (unsigned long)sizeof(short int));
    printf("Size of data types: unsigned short int: %lu\n", (unsigned long)sizeof(unsigned short int));
    printf("Size of data types: unsigned int: %lu\n", (unsigned long)sizeof(unsigned int));
    printf("Size of data types: int: %lu\n", (unsigned long)sizeof(int)); 
    printf("Size of data types: long int: %lu\n", (unsigned long)sizeof(long int)); 
    printf("Size of data types: unsigned long int: %lu\n", (unsigned long)sizeof(unsigned long int));
    printf("Size of data types: signed char: %lu\n", (unsigned long)sizeof(signed char));
    printf("Size of data types: unsigned char: %lu\n", (unsigned long)sizeof(unsigned char));
    printf("Size of data types: float: %lu\n", (unsigned long)sizeof(float));
    printf("Size of data types: double: %lu\n", (unsigned long)sizeof(double));
    printf("Size of data types: long double: %lu\n", (unsigned long)sizeof(long double));

    return 0;
}
