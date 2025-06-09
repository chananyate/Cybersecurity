#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch;

    printf("Enter key ESC to exit \n");
	
    while (1)
    {
	 ch = getchar();

         if ((int)ch == 65)
             printf("A was pressed.\n");
             
         else if ((int)ch == 84)
             printf("T was pressed.\n");
			
         else if ((int)ch == 27)
	     exit(0);

     }
}
