# Exploiting Buffer Overflow in gets() Function

# Vulnerable Code
The following C code contains a buffer overflow vulnerability due to the use of `gets()`, which doesn't enforce input length constraints:

## Objective 
The goal was to exploit a **buffer overflow vulnerability** to bypass authentication. The program checks if the user-provided input in `buff` matches the hardcoded `password` using `strncmp()`. By overflowing `buff`, I aimed to overwrite `password` with my input, causing `strncmp()` to return true and granting unauthorized access.

## Initial Attempt & Stack Protection Issue

At first, when attempting to overflow buff with many 'a' characters, it didn't work. This was because the compiler automatically enabled stack protection by default. The stack protector placed the buff buffer at a higher memory address than the password array. Since memory is written from lower to higher addresses, overflowing buff did not overwrite password. I confirmed this by inspecting the memory addresses via GDB.

## Disabling Stack Protection
To change the memory layout so that `buff` is below `password`, I recompiled the program with:

```bash
gcc -fno-stack-protector -o exploit vulnerable.c
```

This disabled stack protection, allowing memory to be allocated in the order defined in the source code.

## Exploit Execution
Once stack protection was disabled, I attempted the overflow again:

1. Entering **40 times the letter 'a'** only **partially** overwrote `password` due to stack padding.
2. Entering **52 times the letter 'a'** successfully overwrote the entire `password` array.
3. Since `buff` now contained the same values as `password`(20 times tghe letter 'a'), `strncmp(buff, password, 20)` returned `true`, and the program printed:

   ```
   You have successfully entered the password.
   ```

