#include <stdio.h>
#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) (MAX2(MAX2(a, b), c))

int main()
{
	int a, b, c, result;

	printf("Enter three integers: ");
	scanf("%d %d %d", &a, &b, &c);

	result = MAX3(a, b, c);

	printf("%d\n", result);

	return 0;
}