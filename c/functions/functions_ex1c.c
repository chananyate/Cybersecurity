#include <stdio.h>
#include <stdlib.h>

typedef void (*KeyPressed)();

KeyPressed ptr[256];

    void ascii_A()
    {
        printf("A-pressed\n");
    }

    void ascii_T()
    {
        printf("T-pressed\n");
    }

    void ascii_esc()
    {
        exit(0);
    }

    void empty_function()
    {
    
    }
 

int main()
{
    KeyPressed ptr[256];

    unsigned char ch;

    int i;

    for (i = 0; i < 256; ++i)
        ptr[i] = empty_function;

    ptr[65] = ascii_A;

    ptr[84] = ascii_T;

    ptr[27] = ascii_esc;

    printf("Press A or T to display, esc to exit.\n");

    while (1)
    {
        ch = getchar();
        
        ptr[(int)ch]();
    }
    return 0;
}
