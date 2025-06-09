#include <stdio.h>
#define MAX_SIZE 50

void MaxSubarray(int array[], int length);

int main() 
{
    int length, i, array[MAX_SIZE];

    printf("Enter number of elements: ");
    scanf("%d", &length);

    for (i = 0; i < length; ++i) 
    {
        printf("Enter element: ");
        scanf("%d", &array[i]);
    }

    MaxSubarray(array, length);

    return 0;
}

void MaxSubarray(int array[], int length) 
{
    int max_current = array[0];
    int max_global = array[0];
    int i;
    
    for (i = 1; i < length; ++i) 
    {
		if (array[i] > max_current + array[i])
    		max_current = array[i];

		else 
			max_current = max_current + array[i];

        if (max_current > max_global) 
            max_global = max_current;
    }

    printf("The maximum sum is %d\n", max_global);
}
