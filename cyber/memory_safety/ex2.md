## Execution and Findings

1. **Buffer Overflow to Crash Program**
   ```bash
      ./a.out aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
   ```

2. **Fuzzing Execution**

### Install Clang
```bash
sudo apt-get install clang llvm
```
- Clang is a compiler front-end that focuses on performance, ease of use, and producing useful error messages.
- LLVM is the larger project that provides the infrastructure behind Clang and other tools used for compiling code.

### Modify code and remove main

```ex2.c
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) return 0;  
    ```
- This function is called by the fuzzer to process each input. If the input size is zero, we return immediately to avoid unnecessary processing of empty data. Fuzzing focuses on generating non-empty inputs to test for vulnerabilities or crashes in the target code.
- In fuzz testing with libFuzzer, you typically do not need a main function because libFuzzer handles the process of calling the target function (LLVMFuzzerTestOneInput) automatically.

### Compile

- Since libFuzzer is a C++ library

```bash
sudo apt-get install libstdc++-12-dev
```

```bash
clang -fsanitize=fuzzer ex2.c
```
- This option enables fuzzer instrumentation via Clang’s sanitizers.
libFuzzer is a fuzz testing engine, and -fsanitize=fuzzer tells Clang to instrument the program with the necessary hooks to run the program in a fuzzing context. This flag enables code to generate random inputs and monitor for bugs, crashes, or unexpected behavior.

   ```bash
   ./a.out
   ```
   - The fuzzer ran, generating random inputs to test the program.
   - The process encountered a **heap-buffer-overflow**.

3. **AddressSanitizer**
   - A sanitizer is a tool used to detect various types of runtime errors in programs, like memory corruption, undefined behavior, and data races. It helps developers identify and fix security vulnerabilities and bugs in their code.

   ```bash
   clang -fsanitize=address,fuzzer ex2.c
   ```
   - -fsanitize=address: Enables AddressSanitizer (ASan), which is a runtime memory error detector that finds bugs related to memory access (like buffer overflows, use-after-free, memory leaks etc.).

   ```bash
   ./a.out
   ```

   - **Error Message:**

     ```Output
     ==606308==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000051 at pc
     ```
   - **Cause:**
     - `strcpy` attempted to copy data into a heap-allocated buffer that was too small.

4. **Key Information from ASan Output**
- The shadow memory (special markers used by memory sanitizers to track memory regions) confirms this was an overflow (`fa fa 01 fa` pattern).
 
- Shadow bytes: 
0x0c047fff8000: fa fa fd fa fa fa 01 fa fa fa[01]fa fa fa fa fa

- 'fa' represents "heap left redzone" (protected memory around the allocated buffer).
   This indicates the boundary just before and after the allocated memory buffer, used to detect out-of-bounds accesses.
- 'fd' represents "freed heap region" (memory that has been freed but still accessed, indicating a use-after-free).
- '01' represents "partially addressable" memory, which indicates an invalid access is happening in a valid region, potentially indicating an   out-of-bounds read or write that overflows the buffer.  
- The overflow occurs at address 0x602000000051, which is 1 byte past the allocated 64-byte buffer. 
- The error occurs due to an incorrect read or write beyond the bounds of the buffer, causing corruption of the memory redzone (protected memory regions around allocated buffers), which triggers the AddressSanitizer's detection mechanism.
