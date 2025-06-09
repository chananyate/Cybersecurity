#include <stdio.h>

void Print(int a);

struct print_me
{
    int a;
    void (*funcPtr)(int);
};

int main()
{
    int i;

    struct print_me array[10];

    for (i = 0; i < 10; ++i)
    {
        array[i].a = i + 5;
        array[i].funcPtr = Print;
        array[i].funcPtr(array[i].a);
    }

    return 0;
}

void Print(int a)
{
    printf("%d\n", a);
}
