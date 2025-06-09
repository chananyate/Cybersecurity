#include <stdio.h>

int CountSet(int a);

int main()
{
	int number, result;

	printf("Enter a number: ");
	scanf("%d", &number);

	result = CountSet(number);

	printf("There are %d bits set int this number\n", result);

	return 0;
}

int CountSet(int a)
{
	int i, mask, count;

	count = 0;

	for (i = 0; i < 32; ++i)
	{
		mask = 1 << i;

		if (a & mask)
			++count;
	}
	return count;
}