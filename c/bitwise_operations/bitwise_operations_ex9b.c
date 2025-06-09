#include <stdio.h>

int CountSet(int a);

int main()
{
	int number, result;

	printf("Enter a number: ");
	scanf("%d", &number);

	result = CountSet(number);

	printf("There are %d bits set in this number\n", result);

	return 0;
}

int CountSet(int a)
{
	int count;

	count = 0;

	count = ((a & 0xAAAAAAAA) >> 1) + (a & 0x55555555);/*Counts the 1's in bit pairs by alternating single bits in each masking. Sets the even bits to 1 and masks the rest. Then shifts them to the odd position to align them with the odd bits (0x55555555). Then performs + which is both logical "or" and bitwise addition.*/

	a = ((count & 0xCCCCCCCC) >> 2) + (count & 0x33333333);/*Counts the 1's by masking in groups of two, and so on. These phases are done to ensure no set bits are overlooked*/  

	count = ((a & 0xF0F0F0F0) >> 4) + (a & 0x0F0F0F0F); 

	a = ((count & 0xFF00FF00) >> 8) + (count & 0x00FF00FF); 

	count = (a >> 16) + (a & 0x0000FFFF); 

	return count;
}