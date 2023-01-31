# phantasm

An assembly language that compiles to C. (Yeah, it's backwards.)

With phantasm, this is valid C code:

```c
unsigned long factorial(int n) {
        mov     r0      $(1);
        mov     r1      $(n);
loop:
        cmp     r1      $(1);
        jle     done;
        mul     r0      r0      r1;
        sub     r1      r1      $(1);
        jmp     loop;
done:
        return *r0;
}
```

> See [factorial.c](examples/factorial.c) for the full example.

## Installation

Download [phantasm.h](phantasm.h) and include it in your C source files:

```c
#include "phantasm.h"
```

## Usage

### Registers

phantasm has 16 general-purpose registers, numbered `r0` through `r15`. They each hold a 64-bit value, which can interpreted as a signed integer, an unsigned integer, or a pointer (depending on the instruction).

Every phantasm instruction is a C statement, so there is no program counter, stack pointer, etc.

### Operands

Instruction operands can be:

- A register, like `r0`
- An integer constant, like `$(42)`
- An integer variable, like `$(n)`
- A pointer variable, like `$$(str)`

> Using `$` for integer values and `$$` for pointers enables the compiler to catch accidental conversions between integers and pointers.

You can put arbitrarily complex expressions in `$(...)` and `$$(...)`, if you feel like cheating a little.

### Initializing Registers

Use the `mov` instruction:

```c
// put the integer 1 in r0
mov     r0      $(1);

// put the value of the variable n in r1
mov     r1      $(n);
```

### Arithmetic Instructions

Arithmetic instructions have a destination and two operands:

```c
// put r0 + r1 in r2
add     r2      r0      r1;
```

`sub` and `mul` behave analogously.

### Jump Instructions

To write an unconditional jump, define a label (using regular C syntax) and use `jmp`:

```c
loop:
        // ...
        jmp loop;
```

Conditional jumps use `cmp` and a conditional jump instruction:

```c
        // jump to 'done' if r1 <= 1
        cmp     r1      $(1);
        jle     done;
        // ...
done:
```

> The following jump instructions are supported, with the same names and semantics as the [x86 jump instructions](https://en.wikibooks.org/wiki/X86_Assembly/Control_Flow#Jump_Instructions): `jmp`, `je`, `jne`, `jg`, `jge`, `jl`, `jle`, `ja`, `jae`, `jb`, and `jbe`.

### Memory Instructions

Copy a C pointer into a register first, to use as a base address:

```c
// put the value of the pointer in r1
mov     r1      $$(str);
```

Load and store instructions have a destination or source respectively, a base address, and an offset (which is always in bytes):

```c
// load an unsigned byte into r3 from address (r1 + 0)
ldb     r3      r1      $(0);
// store an unsigned byte from r4 into address (r1 + 0)
stb     r4      r1      $(0);
```

> See [strrev.c](examples/strrev.c) for the full example.

Note that the offset must be specified, even if it's zero. In general, if not enough operands are provided to an instruction, it will behave like a no-op.

### Reading Registers from C

Dereference the register name to get an `uint64_t`:

```c
return *r0;
```

## How It Works

After macro expansion, the `factorial` function from earlier looks like this:

```c
unsigned long factorial(int n) {
        mov (&_pasm_registers[0]) (_pasm_immediate_value(1));
        mov (&_pasm_registers[1]) (_pasm_immediate_value(n));
loop:
        cmp (&_pasm_registers[1]) (_pasm_immediate_value(1));
        if ((int64_t) _pasm_left <= (int64_t) _pasm_right) goto done;
        mul (&_pasm_registers[0]) (&_pasm_registers[0]) (&_pasm_registers[1]);
        sub (&_pasm_registers[1]) (&_pasm_registers[1]) (_pasm_immediate_value(1));
        goto loop;
done:
        return *(&_pasm_registers[0]);
}
```

Registers and immediate values become parenthesized expressions of type `uint64_t*`. Non-jump instructions are functions that take a single argument and return a function pointer; this function pointer is immediately called with the next argument. Jump instructions become `goto`s, preceded by an `if` statement if necessary.

## But Why?

Why not?

## To-Do

- Bitwise operations
- More memory instructions
