#include <assert.h>/*for assert*/
#include <stdlib.h>/*for malloc*/
#include <string.h>

char *strdup(const char *str)
{
    char *duplicate = NULL;
   
    assert(NULL != str);

    size_t length;
 
    length = strlen(str);

    duplicate = malloc(length + 1);

    assert(NULL != duplicate);

    while (*str != '\0')
    {
        *duplicate = *str;
        ++str;
        ++duplicate;
    }
    *duplicate = '\0';

    return duplicate;
}
