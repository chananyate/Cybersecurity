#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>/*for printf*/
#include <stdlib.h>/*for getline function*/

int StrnCmp(const char *str1, const char *str2, unsigned long n);

int main()
{
    int StringResult;

    char *str1 = NULL;

    size_t buffer1 = 0; 
   
    ssize_t length1;
    
    char *str2 = NULL;

    size_t buffer2 = 0; 
   
    ssize_t length2;

    unsigned long n;

    printf("Enter string 1: \n");

    length1 = getline(&str1, &buffer1, stdin);  

    assert(length1 != -1);

    assert(NULL != str1);
    
    printf("Enter string 2: \n");

    length2 = getline(&str2, &buffer2, stdin);  

    assert(length2 != -1);

    assert(NULL != str2);

    printf("Enter maximum number of characters to be compared: \n");

    scanf("%lu", &n);

    StringResult = StrnCmp(str1, str2, n);

    if (StringResult == 0)
        printf("The first %lu characters of both strings are equal.\n", n);
    else if (StringResult < 0) 
        printf("The first %lu characters' lexicographical value of str1 is less than str2.\n", n);
    else 
        printf("The first %lu characters' lexicographical value of str1 is greater than str2.\n", n);
    

    free(str1);
    free(str2);

    return 0;

}


int StrnCmp(const char *str1, const char *str2, unsigned long n)
{   
    unsigned long i = 0;

    assert(NULL != str1);
    assert(NULL != str2);

    while (i < n && str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return (int)(str1[i] - str2[i]);
        }
        ++i;
    }

    if (i < n)
    {
        return (int)(str1[i] - str2[i]);
    }

    return 0;
}
