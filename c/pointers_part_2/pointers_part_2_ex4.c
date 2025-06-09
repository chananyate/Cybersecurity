#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>/*for isspace function*/
#include <stdio.h>/*for printf*/
#include <stdlib.h>/*for getline function*/
#include <string.h>/*for strlen function*/

char *Space(const char *str);

int main()
{
    char *string = NULL;

    size_t buffer = 0; 
   
    ssize_t length;

    char *clean_string = NULL;

    printf("Enter string: ");
    
    length = getline(&string, &buffer, stdin);  

    assert(length != -1);

    assert(NULL != string);

    clean_string = Space(string);

    printf("Cleaned up string: %s\n", clean_string);

    free(string);

    free(clean_string);
    
    return 0;

}


char *Space(const char *str)
{
    size_t length;

    int flag;
   
    char *str2 = NULL; 

    char *tmp_str2 = NULL;

    const char *start = NULL;

    const char *end = NULL;

    length = strlen(str) + 1;

    str2 = malloc(length);

    assert(NULL != str2);

    tmp_str2 = str2;

    start = str;

    end = str + length - 1;

    flag = 1;

    while (*str != '\0')
    {
        if ((str == start && isspace(*str)) || (str == end && isspace(*str)) || (flag == 1 && isspace(*str)))
            ++str;

        else
        {
            flag = 0;

            if (isspace(*str))
                flag = 1;

            *str2 = *str;
            ++str2;
            ++str;
        }
        
    }
    *str2 = '\0';
    
    str2 = tmp_str2;

    return str2;
}

