1. Created foo.c

2. Compiled the source file into an object file -
gd -fPIC -c foo.c -o foo.o

3. Created the shared library from the object file - 
gd -shared -o libfoo.so foo.o

4. Created main.c, compiled and linked it with the static library to create the executable - 
gd -o program main.c -L. -lfoo

5. Setting the environment variable for runtime linking
export LD_LIBRARY_PATH=. 