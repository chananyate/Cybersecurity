#include <assert.h> /*for assert function*/
#include <stddef.h> /*for size_t type*/


size_t StrLen(const char *str)
{
    const char *runner = str;

    assert(NULL != str);

    while(*runner != '\0')
        ++runner;

    return (size_t)(runner - str);
}

int StrCmp(const char *str1, const char *str2)
{
    assert(NULL != str1);
    assert(NULL != str2);

    while(*str1 != '\0' && *str1 == *str2)
    {
        ++str1; 
        ++str2;
    }

    return (int)(*str1 - *str2);
}
