# Manual Function Prologue with PIE Compatibility

## Overview
Goal: Perform a function call prologue manually in assembly with PIE (Position Independent Executable) compatibility.


## Initial Problem
In a non-PIE environment, function addresses are fixed, so directly jumping to a specific function using inline assembly was straightforward. However, with PIE enabled, addresses are determined dynamically at runtime, requiring adjustments to resolve them relative to the instruction pointer (RIP).


## Code
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
        "lea finish(%rip), %rax;" 
        "lea return_label(%rip), %rcx;" 
        "push %rcx;"            
        "jmp *%rax;"            
        "return_label:"        
    );
}

int main() {
  func();
  return 0;
}
```

### Viewing Function Addresses
I used `objdump` to disassemble the binary and locate the addresses:

```bash
objdump -d skip64_pie | grep -A30 "<func>"
```

### Output
```
0000000000001204 <func>:
    1204:    f3 0f 1e fa           endbr64 
    1208:    55                    push   %rbp
    1209:    48 89 e5              mov    %rsp,%rbp
    120c:    48 8d 05 56 ff ff ff  lea    -0xaa(%rip),%rax        # 1169 <finish>
    1213:    48 8d 0d 03 00 00 00  lea    0x3(%rip),%rcx          # 121d <return_label>
    121a:    51                    push   %rcx
    121b:    ff e0                 jmp    *%rax
```

### Explanation of Instructions

#### `120c: lea -0xaa(%rip), %rax`
- **Instruction:** Computes the address of `finish` and stores it in `RAX`.
- **Explanation:**
  - `lea` (Load Effective Address) calculates the target address as an offset from `%rip` (the address of the next instruction).
  - `%rip` immediately after this instruction is `0x1213`. The offset `-0xaa` is added to `%rip`, resulting in the address `0x1169`, which is the location of `finish`.

#### `1213: lea 0x3(%rip), %rcx`
- **Instruction:** Computes the address of `return_label` and stores it in `RCX`.
- **Explanation:**
  - The address of `return_label` is calculated relative to `%rip`.
  - `%rip` after this instruction is `0x121a`. Adding the offset `0x3` results in `0x121d`, the location of `return_label`.

#### `121a: push %rcx`
- **Instruction:** Pushes the address of `return_label` (computed in `RCX`) onto the stack.
- **Explanation:**
  - This sets up the return address for when `finish` completes execution and executes a `ret` instruction.

#### `121b: jmp *%rax`
- **Instruction:** Jump to the address stored in `RAX` (calculated as `finish`).


## Return Label Behavior
- **`return_label:`** is a placeholder name, but its position in the code directly after the `jmp` ensures that the compiler assigns it the correct address.
- **Flow:** When `finish` executes a `ret`, the program uses the address at the top of the stack (placed there by `push %rcx`) to return to the `return_label` instruction.


- PIE binaries do not have fixed addresses at runtime; instead, addresses are calculated relative to `%rip`.
- This ensures compatibility across different memory layouts and improves security by enabling address space layout randomization (ASLR).


