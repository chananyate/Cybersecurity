1. Created foo.c

2. Compiled the source file into an object file -
gd -c foo.c -o foo.o

3. Created the static library from the object file - 
ar rcs libfoo.a foo.o

4. Created main.c, compiled and linked it with the static library to create the executable - 
gd -o program main.c -L. -lfoo