#include <string.h>

bool IsPalindrome(const char *str)
{
    size_t length = strlen(str);
    
    const  char *tmp = NULL;

    tmp = (str + length - 1);    

    bool check_string = true;

    while (str < tmp)
    {
        if (*str == *tmp)
        {
            ++str;
            --tmp;
        }
 
        else
        {
            check_string = false;

            break;
        }
     }

    return check_string;
}
