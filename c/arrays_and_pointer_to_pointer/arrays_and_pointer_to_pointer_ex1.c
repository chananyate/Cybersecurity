#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void sum_array(int rows, int columns, int **array, int *result);

int main()
{
    int rows, columns, i ,j;

    int **array = NULL;

    int *result = NULL;

    printf("Enter number of rows: ");
    scanf("%d", &rows);

    printf("Enter number of columns: "); 
    scanf("%d", &columns); 
 
    array = malloc(sizeof(int*)*rows);

    assert(NULL != array);

    for (i = 0; i < rows; ++i)
        array[i] = malloc(sizeof(int)*columns);

    result = malloc(sizeof(int)*rows);

    assert(NULL != result);

     for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < columns; ++j)  
        {
            printf("The element at index [%d][%d] is: ", i , j);
            scanf("%d", &array[i][j]);
        }
    }

    sum_array(rows, columns, array, result);

    free (array);
    free (result);

    return 0;
}


void sum_array(int rows, int columns, int **array, int *result)
{
    int i, j, sum; 

    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < columns; ++j)
            sum += array[i][j];

        result[i] = sum;

        sum = 0;

        printf("%d ", result[i]);
     }
      
}
