# Variables
CC = gcc

CFLAGS = -Wall -g -ansi -pedantic-errors -Wextra -Iinclude

TARGET = stack_test.out

OBJS = test/stack_test.o src/stack.o

# all is the name of the target. $(TARGET) is a dependency for the all target. 
# In this Makefile, $(TARGET) is defined as fooer.out, so all depends on the successful creation of fooer.out.
# when running make without any arguments, make will build the first target listed in the Makefile, which is named all by convention, and executes the commands associated with it.
all: $(TARGET)

# Rule that specifies how to build the executable file
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile stack.o
src/stack.o: src/stack.c include/stack.h
	$(CC) $(CFLAGS) -c src/stack.c -o src/stack.o

# Rule to compile stack_test.o
test/stack_test.o: test/stack_test.c include/stack.h
	$(CC) $(CFLAGS) -c test/stack_test.c -o test/stack_test.o

# Cleans target to remove object files and the executable
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) libstack.a

# Debugs target with extra debug flags
.PHONY: debug
debug: CFLAGS += -DDEBUG
debug: all

# Releases target with optimization flags
.PHONY: release
release: CFLAGS += -O2
release: all

# Creates static library libstack.a
libstack.a: src/stack.o
	ar rcs libstack.a src/stack.o

# Builds the static library and the executable
.PHONY: libstack
libstack: libstack.a 