#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>/*for printf*/
#include <stdlib.h>/*for getline function*/

void StrnCpy(char *copy, const char *original, unsigned long n);

int main()
{
    char *original = NULL;

    size_t buffer = 0; 
   
    ssize_t length;

    unsigned long n;

    char *copy = NULL;

    printf("Enter string: \n");

    length = getline(&original, &buffer, stdin);  

    assert(length != -1);

    assert(NULL != original);

    printf("Enter maximum number of characters to be copied: \n");

    scanf("%lu", &n);

    copy = malloc(sizeof(n + 1));

    assert(NULL != copy);

    StrnCpy(copy, original, n);

    printf("The copied string is: %s\n", copy);

    free(original);
    free(copy);

    return 0;

}

void StrnCpy(char *copy, const char *original, unsigned long n)
{ 
    int i = 0;

    while(i < ((int)n - 1) && *original != '\0')
    {
         *copy = *original;
         ++original;
         ++copy; 
         ++i;
    }
    *copy = '\0';
}
