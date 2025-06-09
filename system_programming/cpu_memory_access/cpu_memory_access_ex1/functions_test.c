#include <stdio.h>
#include "functions.h"

int main()
{
    char buffer[20];
    char src[] = "Hello world!";
    
    Memset(buffer, 'A', sizeof(buffer) - 1);/*used to fill a block of memory with a particular value*/
    buffer[19] = '\0';  
    printf("After Memset: %s\n", buffer);
    
    Memcpy(buffer, src, sizeof(src));/*designed for copying memory between non-overlapping memory regions.*/
    printf("After Memcpy: %s\n", buffer);
    
    Memmove(buffer + 6, buffer, 6);/*it's designed for copying memory between potentially overlapping regions*/ 
    buffer[12] = '\0';  
    printf("After Memmove: %s\n", buffer);
    
    return 0;
}
