#include <ctype.h>/*for toupper*/

int StrCaseCmp(const char *str1, const char *str2)
{
    assert(NULL != str1);
    assert(NULL != str2);

    while(*str1 != '\0' && toupper((unsigned char)*str1) == toupper((unsigned char)*str2))
    {
        ++str1; 
        ++str2;
    }

    return (int)(toupper((unsigned char)*str1) - toupper((unsigned char)*str2));
}
