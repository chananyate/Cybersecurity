#include <stdio.h>
#include <stdlib.h>  /*rand(), srand()*/
#include <time.h>

#define SIZE 5000

void CountingSort(int array[], int size);

int main()
{
	int i, array[SIZE];
	int size = SIZE;
	double time_taken;
	clock_t start_time, end_time;

	srand((unsigned int)time(NULL));

	for (i = 0; i < size; ++i)
		array[i] = (rand() % 100) + 1;

	start_time = clock();

	CountingSort(array, size);

	end_time = clock();

	for (i = 0; i < size; ++i)
		printf("%d ", array[i]);

	printf("\n");

	/*calculates elapsed time in seconds*/
	time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

	printf("Sorting took %f seconds.\n", time_taken);

	return 0;
}