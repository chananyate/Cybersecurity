#include <assert.h>  /*assert*/
#include <stdlib.h>  /*malloc*/
#include <string.h>  /*memcpy*/
#include "sorts_ex7.h"



int Compare(const void *x, const void *y)
{
	const int a = *(const int *)x;
	const int b = *(const int *)y;

	if (a > b)
		return 1;

	else if (a < b)
		return -1;

	else
		return 0;
}


void QuickSort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))
{
	char *temp = NULL;
	size_t j;
	size_t i = 0;

	/*casting to char for byte size operations*/
    char *arr = (char *)base; 

    /*the pivot will be the last element*/
    char *pivot = arr + (nitems - 1) * size;

	/*base case of an array with fewer than 2 elements which means it's already sorted*/
	if (nitems < 2) 
		return;  

    /*partitioning step*/
    for (j = 0; j < nitems - 1; ++j) 
    {
        /*compares current element with the pivot using the comparison function. 
          if arr[j] < pivot then swap arr[i] and arr[j] to ensure that all 
          smaller elements remain on the left side of the pivot.*/
        if (compar(arr + j * size, pivot) < 0) 
        {
			temp = malloc(size);

			assert(NULL != temp);       	

            memcpy(temp, arr + i * size, size);

            memcpy(arr + i * size, arr + j * size, size);

            memcpy(arr + j * size, temp, size);

            free(temp);

            ++i;
        }
    }

    /*places the pivot in its correct position by swapping with arr[i]*/
    temp = malloc(size);

    assert(NULL != temp);       	

    memcpy(temp, arr + i * size, size);

    memcpy(arr + i * size, pivot, size);

    memcpy(pivot, temp, size);

    free (temp);

    /*recursive calls for the two partitions*/
    QuickSort(arr, i, size, compar); /*left*/ 

    QuickSort(arr + (i + 1) * size, nitems - i - 1, size, compar);  /*right*/
}

