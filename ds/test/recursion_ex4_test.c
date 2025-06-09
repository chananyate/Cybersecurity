#include <stdio.h>
#include <assert.h>
#include "recursion_ex4.h"


void test_Strlen() 
{
    assert(Strlen("") == 0);

    assert(Strlen("hello") == 5);

    assert(Strlen("OpenAI") == 6);

    printf("strlen tests passed.\n");
}

void test_Strcmp() 
{
    assert(Strcmp("hello", "hello") == 0);

    assert(Strcmp("hello", "world") < 0);

    assert(Strcmp("world", "hello") > 0);

    assert(Strcmp("", "") == 0);

    assert(Strcmp("a", "b") < 0);

    printf("strcmp tests passed.\n");
}

void test_Strcpy() 
{
    char dest[20];

    assert(Strcpy(dest, "hello") == dest);

    assert(Strcmp(dest, "hello") == 0);

    assert(Strcpy(dest, "") == dest);

    assert(Strcmp(dest, "") == 0);

    printf("strcpy tests passed.\n");
}

void test_Strcat() 
{
    char dest[20] = "Hello";

    assert(Strcat(dest, " World") == dest);

    assert(Strcmp(dest, "Hello World") == 0);

    assert(Strcat(dest, "") == dest);

    assert(Strcmp(dest, "Hello World") == 0);

    printf("strcat tests passed.\n");
}

void test_Strstr() 
{
    char *string = Strstr("hello world", "world");

    printf("%s\n", string);
    
    /*assert(Strstr("hello world", "world") == "world");

    assert(Strstr("hello world", "hello") == "hello world");

    assert(Strstr("hello world", "universe") == NULL);

    assert(Strstr("hello world", "") == "hello world");

    assert(Strstr("", "hello") == NULL);

    printf("strstr tests passed.\n");*/
}

int main() 
{
    test_Strlen();

    test_Strcmp();

    test_Strcpy();

    test_Strcat();

    test_Strstr();

    printf("All tests passed successfully!\n");

    return 0;
}