#include <assert.h>
#include <stdlib.h>
#include <string.h>

char *StrCat(char *dest, const char *src)
{   
    size_t length;

    length = strlen(src) + strlen(dest) + 1;

    char *combined_strings = malloc(length);

    assert(NULL != combined_strings);

    char *starting_point = combined_strings;

    while (*dest != '\0')
    {
        *combined_strings = *dest;
        ++dest;
        ++combined_strings;
    }
   
    while (*src != '\0')
    {
        *combined_strings = *src;
        ++src;
        ++combined_strings;
    }
    *combined_strings = '\0';

    return starting_point;
}
