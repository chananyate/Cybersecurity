#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reversed_string(char *string, int length);

int main()
{
    int length;

    char *string = NULL;

    size_t buffer;

    buffer = 0;

    printf("Enter string: ");
  
    length = getline(&string, &buffer, stdin);  
 
    assert(NULL != string);

    reversed_string(string, length);

    printf("The reversed string is: %s\n", string); 

    free(string);
    
    return 0;
}

void reversed_string(char *string, int length)
{
    char tmp;
 
    char *start = string;

    char *end = string + length - 1;

    while(start < end)
    {
        tmp = *start;
        
        *start = (*end >= 'A' && *end <= 'Z') ? (*end + 32) : *end;
        
        *end = (tmp >= 'A' && tmp <= 'Z') ? (tmp + 32) : tmp;
        
        ++start;

        --end;
     }
}

