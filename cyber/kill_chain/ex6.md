## Overview
Goal: Manually transferring control to a function (finish) while ensuring proper stack setup for a seamless return to the original caller.


## Initial Problem
I attempted to jump to a specific function (`finish`) from another function (`func`) in a program using inline assembly, but encountered a segmentation fault. This required diagnosing and resolving issues with stack management and program flow.


## Final Code
```c
#include <stdio.h>

void finish(int a, int b) {
  if (42 == a) {
    printf("That's an interesting value for a.\n");
  }
  if (0 == b) {
    printf("How mundane.\n");
  }
  if (32767 == a * b) {
    printf("Keep in mind that neither success nor failure is ever final.\n");
  }
  if (8001 == a * b || 9001 == a * b) {
    printf("Realize that while the first 90 percent of code accounts for 90 percent of the development time, so does the last 10 percent of the code.\n");
  }

  printf("You've successfully finished this part of the exercise.\n");
}

void func() {
    asm volatile(
      "mov $0x401206, %rax;"  
      "push %rax;"            
      "jmp 0x401156"          
    );
}

int main() {
  func();
  return 0;
}
```

## Explanation of the Inline Assembly

### Key Instructions
1. **`mov $0x401206, %rax;`**:
   - Loads the address of the instruction immediately after the jmp to `finish` instruction into the `RAX` register. This serves as the return address from the `finish` function.

2. **`push %rax;`**:
   - Pushes the return address onto the stack so that when `finish` completes and executes a `ret`, the program knows where to resume execution.

3. **`jmp 0x401156;`**:
   - Jumps to the address of the `finish` function (`0x401156`), bypassing the normal function call mechanism.


## Compilation Command
```bash
gcc -Wall -Wextra -g3 -no-pie skip_instructions_64.c -o skip64
```

### Compilation Flags
- **`-Wall`**: Enables all standard warnings.
- **`-Wextra`**: Enables additional warnings.
- **`-g3`**: Includes debugging information in the binary.
- **`-no-pie`**: Disables Position Independent Executable (PIE) mode, ensuring predictable function addresses in the binary.

---

## Debugging the Binary
### Viewing Function Addresses
We used the `objdump` utility to inspect the disassembled code and locate function addresses:

```bash
objdump -d skip64 | grep -A30 "<finish>"
```

