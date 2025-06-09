#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void bitsOn(unsigned int *array, int n);


int main()
{

	int n, i;

	unsigned int *array = NULL;

	printf("Enter a number of elements:\n ");
	scanf("%d", &n);

	array = malloc(sizeof(unsigned int)*n);

	assert(NULL != array);

	for (i = 0; i < n; ++i)
	{
		printf("Enter a number:");
		scanf("%u", array + i);
	}

    bitsOn(array, n);

    free(array);

	return 0;
}

void bitsOn(unsigned int *array, int n)
{
	int count, i;
	unsigned int bit;

	for (i = 0; i < n; ++i)
	{
		bit = 1;

		count = 0;

		while (bit <= *(array + i))
		{
			if ((*(array + i) & bit) != 0)
				++count;

			bit <<= 1;
		}

		if (count == 3)
			printf("%u\n", *(array + i));

	}
}