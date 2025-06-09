First time around, make did this:
gcc -Wall -g -c main.c
gcc -Wall -g -c foo.c
gcc main.o foo.o -o fooer.out

After, typing touch foo.c, make did this:
gcc -Wall -g -c foo.c
gcc main.o foo.o -o fooer.out

The reason is that in my makefile In your Makefile, the dependencies are defined such that:

foo.o depends on foo.c and foo.h.
main.o depends on main.c and foo.h.
$(TARGET) (which is fooer.out) depends on $(OBJS) (which includes main.o and foo.o).

So when foo.c is touched foo.o becomes out-of-date because its dependency foo.c was modified. Thus, foo.o is recompiled.
main.o does not need recompilation because main.c was not modified.