
## **Code Context**

The program consists of:
1. A constant variable `x` at `AREA1`.
2. Function calls creating stack frames: `main()` → `func1()` → `func2()`.
3. Each function declares a `long` variable on the stack.

Since `x` is declared as `const long x = 1`, it is immutable. However, we can manipulate its memory using pointers.

## **Change the value of `x` to 1337 without using the symbol `x` in the code.**

```c
*(long *)(&y + 1) = 1337; // Modify x indirectly
```

### **Explanation**
- `x` and `y` are declared in `main()`. `x` (a `const`) is placed right before `y` on the stack.
- `&y + 1` points to the memory location before `y` (where `x` resides).
- We force-cast this address as a `long` pointer and overwrite the value of `x`.





