#include <stdio.h>

int CountCouples(int number);

int main()
{
	int number;

	printf("Enter number: ");
	scanf("%d", &number);

	printf("There are %d couples of set bits in this number.\n", CountCouples(number));

	return 0;
}

int CountCouples(int number)
{
	int i, mask, count, flag;

	count = 0;
	flag = 0;

	for(i = 0; i < 32; ++i)
	{
		mask = 1 << i;

		if((number & mask) && (flag == 1))
			++count;

		if (number & mask)
			flag = 1;
		else
			flag = 0;
	}

	return count;
}