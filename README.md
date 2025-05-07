# x86-64 Assembly Language (Intel Syntax) Overview

This document provides a basic overview of x86-64 assembly language using Intel syntax. It covers fundamental syntax, registers, common instructions, memory addressing, and control flow. This is often relevant for projects like `libasm` or for understanding low-level programming.

## Basic Syntax

Intel syntax for x86-64 assembly generally follows the `mnemonic destination, source` pattern for instructions.

* **Case Insensitivity**: Instructions, register names, and directives are generally case-insensitive (e.g., `MOV` is the same as `mov`). However, labels (see below) are often case-sensitive depending on the assembler (e.g., NASM, MASM). Consistency is recommended.
* **Comments**: Comments begin with a semicolon (`;`) and extend to the end of the line. They are ignored by the assembler.
    ```assembly
    ; This is a comment
    mov rax, 60 ; syscall number for exit
    ```
* **Labels**:
    Labels are symbolic names followed by a colon (`:`) that mark a specific memory address within your code or data sections. They make code more readable and maintainable by allowing you to refer to locations symbolically instead of using hardcoded addresses.

    * **Purpose**:
        * **Jump/Call Targets**: Labels serve as destinations for control flow instructions like `jmp` (jump), `call` (call procedure), and conditional jumps (e.g., `jz`, `jne`).
            ```assembly
            start_loop:
                ; ... some code ...
                dec rcx
                jnz start_loop ; Jump to start_loop if rcx is not zero
            ret
            ```
        * **Data Addressing**: Labels can define the starting address of data items or variables. The assembler replaces the label with the actual memory address where the data begins.
            ```assembly
            section .data
            my_variable: db 10      ; my_variable is a label for a byte initialized to 10
            message:     db 'Hello, World!', 0 ; message labels a null-terminated string

            section .text
            load_data:
                mov al, [my_variable] ; Load the byte from the address of my_variable into al
                mov rsi, message      ; Load the address of message into rsi (e.g., for printing)
            ```
    * **Naming**: Label names typically consist of alphanumeric characters (a-z, A-Z, 0-9) and underscores (`_`). They usually cannot start with a number. It's good practice to use meaningful names.
    * **Case Sensitivity**: Most assemblers treat labels as **case-sensitive** (e.g., `MyLabel` and `mylabel` would be different labels).
    * **Scope**: Labels are typically local to the file they are defined in. To make a label accessible from other object files during linking (global scope), you usually need to use a directive like `global label_name`. The `_start` label in many Linux examples is often declared global because it's the entry point for the program.
        ```assembly
        global main_function ; Makes main_function visible to the linker

        main_function:
            ; ... code ...
            ret
        ```
* **Instructions**: Instructions are mnemonics that represent machine operations. They may take zero, one, or more operands.
    ```assembly
    mnemonic ; No operands
    mnemonic operand1 ; One operand
    mnemonic operand1, operand2 ; Two operands
    ```
* **Operands**: Operands are the data that instructions operate on. They can be:
    * **Registers**: CPU's internal storage locations (e.g., `rax`, `rcx`).
    * **Memory Locations**: Addresses in RAM (e.g., `[rbx]`, `[my_variable]`).
    * **Immediate Values**: Constant values directly encoded into the instruction (e.g., `10`, `0xFF`).

    In Intel syntax, if an instruction takes two operands, the first operand is usually the **destination** and the second is the **source**.
    ```assembly
    mov rax, 10 ; rax (destination) = 10 (source)
    add rbx, rax ; rbx (destination) = rbx + rax (source)
    ```

## Registers (X86-64)

x86-64 architecture provides a set of 64-bit general-purpose registers (GPRs).

### Traditional Registers

These registers have historical significance from earlier 16-bit and 32-bit architectures (e.g., `AX`, `EAX` are the lower parts of `RAX`).

* **`RAX`**: **Accumulator**. Often used for arithmetic operations, and conventionally holds the return value of a function. In `MUL`/`DIV` operations, it's used for the dividend and stores part of the result.
* **`RBX`**: **Base Register**. A general-purpose register. In some calling conventions (like System V ABI used on Linux/macOS), it's a **callee-saved** register, meaning a function must preserve its value.
* **`RCX`**: **Counter Register**. Often used as a loop counter. In System V ABI, it's used to pass the 4th integer argument to functions.
* **`RDX`**: **Data Register**. Used in conjunction with `RAX` for `MUL`/`DIV` operations (e.g., storing the divisor or part of the result/remainder). In System V ABI, it's used to pass the 3rd integer argument to functions.
* **`RSP`**: **Stack Pointer**. Points to the top of the current stack. It's modified by instructions like `PUSH`, `POP`, `CALL`, and `RET`.
* **`RBP`**: **Base Pointer**. Often used to point to the base of the current function's stack frame, allowing stable access to local variables and parameters. In System V ABI, it's a **callee-saved** register.

*(Note: `RSI` (Source Index) and `RDI` (Destination Index) are also traditional registers, often used for string operations and as the 2nd and 1st function arguments respectively in System V ABI.)*

### Recent Registers (R8-R15)

These are additional general-purpose registers introduced with the x86-64 architecture.

* **`R8`, `R9`**: Used to pass the 5th and 6th integer arguments to functions (in System V ABI).
* **`R10`, `R11`**: Typically used as temporary registers (**caller-saved**). `R10` can sometimes be used for passing a static chain pointer for nested functions.
* **`R12`, `R13`, `R14`, `R15`**: These are **callee-saved** registers. Functions that use them must preserve their original values.

## Data Types and Sizes

When accessing memory, you often need to specify the size of the data being accessed. Common size directives in Intel syntax include:

* `BYTE PTR`: 1 byte
* `WORD PTR`: 2 bytes
* `DWORD PTR`: 4 bytes (Double Word)
* `QWORD PTR`: 8 bytes (Quad Word)

Example:
```assembly
mov BYTE PTR [my_variable], 5  ; Move the byte value 5 into my_variable
mov QWORD PTR [rax], rbx       ; Move the 8-byte value from rbx into memory at address rax
