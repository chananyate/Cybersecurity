1. Created foo.c

2. Compiled the source file into an object file -
gd -fPIC -c foo.c -o foo.o

3. Created the shared library from the object file - 
gd -shared -o libfoo.so foo.o

4. Created the main.c file with dlopen dlsym. Then I compiled and linked it with the dynamic library libdl which provides the dlopen, dlsym and dlclose functions - 
gd -o program main.c -ldl