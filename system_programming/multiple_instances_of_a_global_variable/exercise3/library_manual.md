1. Created headers.h

   Created the c file

   Compiled the source file into an object file -
   gd -c c_file.c -o file.o

   Created the static library from the object file -
   ar rcs libfile.a file.o

2. Created foo.c:
    
   #include "headers.h"

   void Foo(void)
   {
    PrintGlobalVariableAddress();
   }
   
   Compiled and linked to create the shared library:
   gcc -shared -o libfoo.so -fPIC foo.c -L. -lfile

   Created foo2.c:
  
   #include "headers.h"

   void Foo2(void)
   {
     PrintGlobalVariableAddress();
   }

    Compiled and linked to create the second shared library:
    gcc -shared -o libfoo2.so -fPIC foo2.c -L. -lfile

3.  Created the executable main.c:
   
#include <stdio.h>
#include "headers.h"
#include <dlfcn.h>

int main() 
{
    void *handle1, *handle2;
    void (*Foo)(void);
    void (*Foo2)(void);

    handle1 = dlopen("./libfoo.so", RTLD_LAZY);
    if (!handle1)
    {
        printf("Failed to load libfoo.so: %s\n", dlerror());
        return 1;
    }

    Foo = dlsym(handle1, "Foo");

    Foo();

    handle2 = dlopen("./libfoo2.so", RTLD_LAZY);
    if (!handle2)
    {
        printf("Failed to load libfoo2.so: %s\n", dlerror());
        return 1;
    }

    Foo2 = dlsym(handle2, "Foo2");

    Foo2();

    dlclose(handle1);

    dlclose(handle2);

    return 0;
}

   Compiled the executable:
   gcc main.c -L. -lfile -ldl -o main

   Ran the executable:
   ./main
