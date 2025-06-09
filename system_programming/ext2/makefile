CC = gcc
CFLAGS = -std=c90 -pedantic -Wall -Iinclude
OBJ = ext2.o ext2_test.o
TARGET = ext2_test

# The default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile ext2.c into ext2.o
ext2.o: src/ext2.c include/ext2.h
	$(CC) $(CFLAGS) -c src/ext2.c -o ext2.o

# Rule to compile ext2_test.c into ext2_test.o
ext2_test.o: test/ext2_test.c include/ext2.h
	$(CC) $(CFLAGS) -c test/ext2_test.c -o ext2_test.o

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

# This ensures that make recognizes 'all' and 'clean' as targets that 
# should always be executed, preventing any confusion with actual files named 'all' or 'clean'.
.PHONY: all clean
