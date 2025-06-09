#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int main()
{
    char **buffer = NULL;

    char *tmp = NULL;

    int i, environ_count;
  
    environ_count = 0;

    while (*(environ + environ_count) != NULL)
        ++environ_count;

    buffer = malloc(sizeof(char *)*(environ_count + 1));

    assert(NULL != buffer);

    for (i = 0; i < environ_count; ++i)
    {   
        *(buffer + i) = malloc(strlen(environ[i]) + 1);

        assert(NULL != *(buffer + i));

        strcpy(*(buffer + i),*(environ + i));

        tmp = *(buffer + i);

        while (*tmp != '\0')
        {
             *tmp = tolower(*tmp); 
             ++tmp;
        }
        
        printf("%s\n", buffer[i]);
   
        free(buffer[i]);
 
        buffer[i] = NULL;
    }
 
    free(buffer);

    buffer = NULL;

    return 0;
}
