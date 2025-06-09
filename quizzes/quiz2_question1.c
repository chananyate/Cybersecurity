#include <stdio.h>

void TF(int num) 
;

int main()
{
    int num;

    printf("Enter number: ");
    scanf("%d", &num); 

    TF(num);

    return 0;    
    
}

void TF(int num)
{
     int i;

     for (i = 1; i <= num; i++)
     {
         if ((i % 3 == 0) && (i % 5 == 0))
             printf("TF\n");

         else if (i % 3 == 0) 
             printf("T\n");
 
         else if (i % 5 == 0)
             printf("F\n");

         else printf("%d\n", i); 
     }
} 
