#include <stdio.h>
#include <dlfcn.h>
#include "headers.h"

int main() 
{

    void *handle1, *handle2;
    void (*foo)(), (*foo2)();

    handle1 = dlopen("./foo.so", RTLD_LAZY);
    if (!handle1) 
    {
        fprintf(stderr, "Error opening foo.so: %s\n", dlerror());
        return 1;
    }

    handle2 = dlopen("./foo2.so", RTLD_LAZY);
    if (!handle2) 
    {
        fprintf(stderr, "Error opening foo2.so: %s\n", dlerror());
        dlclose(handle1);
        return 1;
    }

    foo = dlsym(handle1, "foo");/*Used to retrieve the address of foo function*/
    foo2 = dlsym(handle2, "foo2");

    if (!foo || !foo2)
    {
        fprintf(stderr, "Error loading functions: %s\n", dlerror());
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    foo();/*call the functions*/
    foo2();

    dlclose(handle1);
    dlclose(handle2);

    return 0;
}
