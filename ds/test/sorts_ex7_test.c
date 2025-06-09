#include <stdio.h>   /*printf*/
#include <stdlib.h>  /*rand(), srand(), size_t*/
#include <time.h>    /*clock*/
#include "sorts_ex7.h"

#define SIZE 5000

int main()
{
	size_t i;
	int array[SIZE];
	double time_taken;
	clock_t start_time, end_time;

	srand((unsigned int)time(NULL));

	for (i = 0; i < SIZE; ++i)
		array[i] = (rand() % 100) + 1;

	start_time = clock();

	QuickSort(array, SIZE, sizeof(int), Compare);

	end_time = clock();

	printf("Sorted array:\n");

	for (i = 0; i < SIZE; ++i)
		printf("%d ", array[i]);

	printf("\n");

	/*calculates elapsed time in seconds*/
	time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

	printf("Sorting took %f seconds.\n", time_taken);

	return 0;
}