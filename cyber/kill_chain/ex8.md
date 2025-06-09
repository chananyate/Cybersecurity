
# Crashing the Program `make_this_crash`

This document outlines the steps taken to crash the program `make_this_crash` using three different methods: resource starvation, instruction pointer manipulation, and buffer overflow.

## Step 1: Preparing the Executable

Before running or debugging the program, I ensured the executable has the correct permissions to run. I used the `chmod` command:

```bash
chmod +x make_this_crash
```

which allows the program to be executed.


## Step 2: Crashing Methods

### **1. Resource Starvation**
I limited the virtual memory available to the program using the `ulimit` command. This forced the program to fail due to insufficient memory.

1. In the terminal:
   ```bash
   ulimit -v 100
   ```
   This sets the virtual memory limit to 100 KB.
   
2. Ran the program:
   ```bash
   ./make_this_crash
   ```
   ```output
   segmentation fault
   ```

3. The program crashes because it cannot allocate the required memory.


### **2. Setting the Instruction Pointer to 0 in GDB**

I manipulated the program's instruction pointer (`RIP`) to force it to execute invalid memory.

1. Launched the program in GDB:
   ```bash
   gdb ./make_this_crash
   ```

2. Started the program:
   ```gdb
   b main
   run
   ```

3. After the program started, I set the instruction pointer to `0`:
   ```gdb
   set $rip = 0
   ```

4. Continued execution:
   ```gdb
   continue
   ```

5. The program crashes because it attempts to execute invalid memory at address `0x0`.

### **3. Buffer Overflow in GDB**

I caused a buffer overflow by overwriting the `str` buffer in GDB with excessive data.

1. Launched the program in GDB:
   ```bash
   gdb ./make_this_crash
   ```

2. Started the program:
   ```gdb
   b main
   run
   ```

3. Located the `str` buffer:
   ```gdb
   info locals
   ```

   output:
   ```
   env_var = 0x0
   str = '\000' <repeats 63 times>
   ```
   ```gdb 
   p &str
   ```

   ```output
   $1 = (char (*)[64]) 0x7fffffffdf60
   ```

4. Injectd data to overflow the `str` buffer.

   ```gdb
   set {char[128]} 0x7fffffffdf60 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
   ```

5. Continued the program:
   ```gdb
   continue
   ```

6. ```output
   Continuing.
   The received string was: ubuntu:GNOME

   Program received signal SIGSEGV, Segmentation fault.
   ```

The program crashes due to memory corruption caused by the buffer overflow.



