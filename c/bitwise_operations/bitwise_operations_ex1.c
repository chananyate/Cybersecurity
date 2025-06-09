#include <stdio.h>

long pow2(unsigned int x, unsigned int y);

int main()
{
	unsigned int a;
	unsigned int b;

	printf("Enter two numbers: ");
	scanf("%u %u", &a,&b);

	printf("The result of multiplying the first number by 2 to the power of the second number is %ld\n", pow2(a,b));
	return 0;
}


long pow2(unsigned int x, unsigned int y)
{
	return x << y;/*since it's 2^y it basically means shif x by y */
}	