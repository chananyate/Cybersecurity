# Assembly Manipulation and Compiling with `-m32` Flag

This document provides a step-by-step summary of how I modified a C program and used assembly instructions to achieve the desired output, "num is now 3".

## Compilation Command

I used the following compilation command:

```bash
gcc -Wall -Wextra -g3 -m32 -no-pie skip_instructions.c -o skip
```

### Flags Explanation:
1. **`-Wall`**: Enables most of the commonly used warnings.
2. **`-Wextra`**: Enables additional warnings not covered by `-Wall`.
3. **`-g3`**: Includes debug information in the output binary for use with tools like `gdb`.
4. **`-m32`**: Compiles the program in 32-bit mode.
5. **`-no-pie`**: Disables Position Independent Executable (PIE) mode, allowing fixed memory addresses in the code.


## Installing Libraries for `-m32`

To use the `-m32` flag, we needed to install the required 32-bit libraries on our system. The following commands were executed:

```bash
sudo apt-get install gcc-multilib g++-multilib
sudo apt-get install libc6-dev-i386
```

## Viewing Assembly Instructions

I used the `objdump` command to inspect the disassembled code of the compiled binary. This allowed me to view the addresses and assembly instructions for the `main` and `func` functions. I used -A30 to display 30 lines of context.

```bash
objdump -d skip | grep -A30 "<func>"
```

### Observed Functions:
I examined the `main` and `func` functions and noted the relevant instructions and memory addresses.


## Modifications to the Code

I modified the `func` function to skip its normal execution and jump back to a specific part of the `main` function to ensure that the program prints "num is now 3". 


### Modified Code:
```c
void func(long a, long b) {
  asm volatile(
      "mov %ebp, %esp;"  
      "pop %ebp;"       
      "jmp 0x80491fd"    
  );
  printf("%ld", b);
  printf("%ld", a);
}
```

### Explanation of Assembly Instructions:
1. **`mov %ebp, %esp;`**: Restores the stack pointer to match the base pointer.
2. **`pop %ebp;`**: Restores the caller's base pointer.
3. **`jmp 0x80491fd;`**: Jumps directly to the desired instruction in the `main` function and bypassing the regular execution flow in `func`. This address 0x80491fd is the address of the instruction right after this instruction: "movl $0x4,-0xc(%ebp)" (which is the instruction responsible for assigning the value 4 to "num"), thus bypassing this assignment as well as skipping over the two "printf" function calls within func(). Consequently "num" remains 3 since it was assigned before our newly added instructions.

