#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i, flag, size, count;
   
    int *soldier_alive = NULL;

    printf("Enter number of soldiers: ");
    scanf("%d", &size);
    
    count = size;
 
    soldier_alive = malloc(sizeof(int)*size);
   
    assert(NULL != soldier_alive);

    for (i = 0; i < size; ++i)
        soldier_alive[i] = 1;
       
    i = 1;

    flag = 0;

    while (size > 1)
    {
        while (soldier_alive[i] && flag == 0)
        {
            soldier_alive[i] = 0;
            flag = 1;
            --size;
        }

        if (soldier_alive[i] && flag == 1)
            flag = 0;

        i = (i + 1) % count;
    }
     for (i = 0; i < count; ++i)
     {
        if (soldier_alive[i])
            printf("Last remaining soldier is number %d\n", i + 1);
     }
     
     free(soldier_alive);
     
     return 0;
}

