#include <assert.h>
#include <stdlib.h>
#include <string.h>

char *StrnCat(char *dest, const char *src, size_t n)
{
    size_t dest_length; 

    size_t src_length; 

    size_t total_length;

    dest_length = strlen(dest);

    src_length = strlen(src);

    total_length = dest_length + (n < src_length ? n : src_length) + 1;

    char *combined_strings = malloc(total_length);

    assert(NULL != combined_strings);

    char *starting_point = combined_strings;

    while (*dest != '\0')
    {
        *combined_strings = *dest;
        ++dest;
        ++combined_strings;
    }
    
    int i;

    for (i = 0; i < n && *src != '\0'; ++i)
    {
        *combined_strings = *src;
        ++src;
        ++combined_strings;
    }

    *combined_strings = '\0';

    return starting_point;
}
 
