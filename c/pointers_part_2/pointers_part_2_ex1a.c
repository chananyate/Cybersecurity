#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>/*for printf*/
#include <stdlib.h>/*for getline function*/

void StrCpy(char *copy, const char *original);

int main()
{
    char *original = NULL;

    size_t buffer = 0; 
   
    ssize_t length;

    char *copy = NULL;

    printf("Enter string: ");
    
    length = getline(&original, &buffer, stdin);  

    assert(length != -1);

    assert(NULL != original);

    copy = malloc(sizeof(length + 1));

    assert(NULL != copy);

    StrCpy(copy, original);

    printf("The copied string is: %s\n", copy);

    free(original);
    free(copy);

    return 0;

}

void StrCpy(char *copy, const char *original)
{
    while(*original != '\0')
    {
         *copy = *original;
         ++original;
         ++copy; 
    }
    *copy = '\0';
}
