#include <stdio.h>
#include <stddef.h>

#define SIZEOF_TYPE(data_type) ((size_t)((char*)(((data_type*)0 + 1)) - (char*)((data_type*)0)))/*Creates a pointer to data_type that points to address 0. Adding 1 moves it forward by sizeof(data_type). Casting it to char ensures we get the exact size in bytes. Otherwise we would get the number of elements, not bytes.*/


int main()
{
	size_t size;

	struct StructureSize{
		char c;
		char a;
		double b;
		char d;
	};

	size = SIZEOF_TYPE(struct StructureSize); 

	printf("The size of the structure is: %lu\n", size);

	return 0;
}