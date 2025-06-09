#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch;

    printf("Enter T or A to display or ESC to exit \n");

    while (1)
    {
         ch = getchar();

         switch ((int)ch)
         {
             case 65: printf("A was pressed.\n"); break; 

             case 84: printf("T was pressed.\n"); break;

             case 27: exit(0); break;
         }
    }   
}
