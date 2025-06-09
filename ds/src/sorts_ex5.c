#include <stdlib.h>

/*int array[] = {5, 1, 9, 6, 4, 3, 6, 1, 6};
int count_array = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int count_array = {0, 2, 0, 1, 1, 1, 3, 0, 0, 1};
int count_array = {0, 2, 2, 3, 4, 5, 8, 8, 8, 9};
int output_array = { , , , , , , , , };*/

void CountingSort(int array[], int size)
{
	int i, range;
	int *count_array = NULL;
	int *output_array = NULL;
	int min = array[0];
	int max = array[0];

	for (i = 0; i < size; ++i)
	{
		if (array[i] < min)
			min = array[i];

		else if (array[i] > max)
			max = array[i];
	}

	range = max - min + 1;

    count_array = malloc(range * sizeof(int));

	for (i = 0; i < range; ++i)
		count_array[i] = 0;

	for (i = 0; i < size; ++i)
		/*subtract min to handle negative values*/
		++count_array[array[i] - min];

	for (i = 1; i < range; ++i)
		count_array[i] += count_array[i - 1];

	output_array = malloc (size * sizeof(int));

	/*accessing the elements from the right side of the array and decrementing the cumulative count_array
	  ensures stability (which means that elements with equal values maintain their relative order 
	  in the sorted array as they had in the original array)*/
	for (i = size - 1; i >= 0; --i)
	{
		output_array[count_array[array[i] - min] - 1] = array[i];
		--count_array[array[i] - min];
	}

	for (i = 0; i < size; ++i)
		array[i] = output_array[i];

	free(count_array);
	free(output_array);
}