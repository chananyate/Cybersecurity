#include <stdio.h>

int IsSumFound(int array[], int sum, int length, int *first_element, int *last_element);

int main()
{
    int length;

    int sum;

    int *ptr1 = NULL;

    int *ptr2 = NULL;

    int array[] = {1, 5, 7, 8, 12, 14, 16, 19, 22};

    length = sizeof(array)/sizeof(array[0]);

    sum = 24;

    IsSumFound(array, sum, length, ptr1, ptr2);

    return 0;
}

int IsSumFound(int array[], int sum, int length, int *first_element, int *last_element)
{   
    first_element = array;

    last_element = array + length - 1;

    while (last_element > first_element)
    {
        if (*last_element + *first_element > sum)
            --last_element;
 
        else if(*last_element + *first_element < sum)
            ++first_element; 

        else
        {
            printf("Sum found for indices %ld and %ld\n", first_element - array, last_element - array);
            
            return 1;
        }
     }
     printf("Sum not found\n");

     return 0;
}
