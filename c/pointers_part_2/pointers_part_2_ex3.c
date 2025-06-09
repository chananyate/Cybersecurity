#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void seven_boom(int *start, int *end);

int main()
{
    int first;

    int last;
    
    int i;

    int range;

    int *space_for_range = NULL;

    printf("Enter a starting point of range: ");
    scanf("%d", &first);

    printf("Enter an end point of range: ");
    scanf("%d", &last);

   if (first > last)
   {   
        printf("end point must be greater than starting point");
        return 1;
    }   

    range = last - first + 1;

    space_for_range = malloc(sizeof(int) * range);

    assert(NULL != space_for_range);

    for (i = 0; i < range; ++i)
        *(space_for_range + i) = first + i;

    seven_boom(space_for_range, space_for_range + range);

    free(space_for_range);

    return 0;
}


void seven_boom(int *start, int *end)
{
    int *runner = NULL;
  
    int tmp;

    runner = start;
    

    while (runner != end)
    {   
        tmp = *runner;
        if (tmp % 7 == 0)
            printf("BOOM\n");
       
        else
        { 
            while (tmp)
            {
                if (tmp % 10 == 7)
                {  
                     printf("BOOM\n");
                     break;
                }
 
                tmp /= 10;
            }

            if (tmp == 0) 
                printf("%d\n", *runner);
         }

          ++runner;
     }
}
