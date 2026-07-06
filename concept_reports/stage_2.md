# BareMetalToECU – Stage 2
# Firmware Build & Boot Pipeline

> Mission:
>
> Understand how a C program becomes running firmware on an MCU.
>
> Guiding Question:
>
> **How does `main.c` become a running embedded program?**

---

# Stage Status

✅ Complete

This stage establishes the mental model for everything that follows.

From this point onward, every hardware topic (GPIO, UART, interrupts, drivers, Linux) builds on the concepts learned here.

---

## 🟢 Must Know

- Compiler
- Assembler
- Linker
- Linker Script
- Object files
- ELF vs BIN
- Flash vs RAM
- .text
- .rodata
- .data
- .bss
- Reset_Handler
- Vector Table
- Symbols
- Linker Symbols
- Runtime Address
- File Offset

## 🟡 Understand

- Bootloader
- VTOR
- Weak Symbols
- ELF Section Headers
- Symbol Table

## 🔴 Stop Here

- ELF binary format
- Relocation encoding
- Exact VTOR bits
- ELF parser internals
- Every linker directive

---

# Big Picture

```
main.c
    │
Compiler
    │
main.o

startup.s
    │
Assembler
    │
startup.o

main.o + startup.o + linker.ld
             │
           Linker
             │
        firmware.elf
             │
      arm-none-eabi-objcopy
             │
        firmware.bin
             │
     ST-Link / OpenOCD
             │
         MCU Flash
             │
Power On / Reset
             │
      Vector Table
             │
      Reset_Handler
             │
Copy .data
Zero .bss
             │
          main()
```

---

# Responsibilities

## main.c

Defines the application.

Contains:

- functions
- global variables
- static variables

The compiler decides what section every object belongs to.

Example:

```c
int counter = 5;
```

↓

```
.data
```

Example:

```c
int flag;
```

↓

```
.bss
```

Example:

```c
int main()
```

↓

```
.text
```

---

## startup.s

Defines how the MCU starts.

Contains:

- vector table
- Reset_Handler

Reset_Handler performs three jobs:

1. Copy `.data` from Flash to RAM.
2. Zero `.bss`.
3. Call `main()`.

Most important realization:

> startup.s initializes **memory regions**, not variables.

It has no idea what `counter` or `rpm` are.

It simply copies:

```
Flash .data

↓

RAM .data
```

using:

```
_sidata
_sdata
_edata
```

---

## linker.ld

Describes the MCU memory map.

It tells the linker:

- where Flash starts
- where RAM starts
- where output sections belong
- to create helper symbols

Example:

```
.text > FLASH

.data > RAM AT > FLASH

.bss > RAM
```

Important:

The linker script does NOT create Flash or RAM.

The MCU already has Flash and RAM.

The linker script describes the hardware to the linker.

---

# Tool Responsibilities

Compiler

Determines:

"What kind of thing is this?"

Assembler

Turns ARM assembly into object code.

Linker

Determines:

"Where does everything live?"

Objcopy

Converts ELF into a raw binary suitable for programming Flash.

---

# Sections

## .text

Executable instructions.

Contains:

- Reset_Handler
- main
- functions

Lives in Flash.

---

## .rodata

Read-only data.

Contains:

- const variables
- string literals

Lives in Flash.

---

## .data

Initialized writable variables.

Example:

```c
int counter = 5;
```

Flash stores:

```
counter = 5
```

Reset_Handler copies it into RAM.

RAM contains:

```
counter = 5
```

Later:

```c
counter = 42;
```

Flash:

```
5
```

RAM:

```
42
```

This was one of the biggest conceptual milestones.

---

## .bss

Zero-initialized writable variables.

Example:

```c
int flag;
```

Lives only in RAM.

Reset_Handler zeroes it.

---

# Symbols

A symbol is a named address.

Examples:

Compiler:

```
main
counter
```

Assembler:

```
Reset_Handler
vector_table
```

Linker:

```
_sidata
_sdata
_edata
_sbss
_ebss
```

Most important realization:

The linker script requests these symbols.

The linker creates them while laying out memory.

---

# Linker Symbols

```
_sidata
```

Start of initialized `.data` image in Flash.

```
_sdata
```

Start of runtime `.data` in RAM.

```
_edata
```

End of runtime `.data` in RAM.

Startup code effectively performs:

```
copy

_sidata

↓

_sdata

↓

until

_edata
```

---

# How the Linker Determines _edata

Suppose:

```c
int counter = 5;
```

Compiler produces:

```
main.o

.data

4 bytes
```

Linker starts `.data`:

```
0x20000000
```

Places counter.

Location counter advances to:

```
0x20000004
```

Now linker executes:

```
_edata = .
```

Therefore:

```
_edata = 0x20000004
```

The linker did NOT receive `_edata` from main.o.

It derived it while laying out memory.

---

# Most Important Mental Model

Compiler

↓

Determines WHAT everything is.

Linker Script

↓

Determines WHERE everything should go.

Linker

↓

Assigns addresses and builds the firmware.

Startup

↓

Initializes memory before C runs.

---

# ELF Mental Model

```
                    firmware.elf

+------------------------------------------------------+
|                                                      |
| ELF Header                                           |
|                                                      |
+------------------------------------------------------+
|                                                      |
| Raw Section Bytes                                    |
|                                                      |
| .isr_vector                                          |
|    vector_table                                      |
|                                                      |
| .text                                                |
|    Reset_Handler                                     |
|    delay                                             |
|    main                                              |
|                                                      |
| .rodata                                              |
|                                                      |
| .data                                                |
|    initial bytes for counter                         |
|                                                      |
| .debug_*                                             |
|                                                      |
+------------------------------------------------------+
|                                                      |
| Symbol Table (.symtab)                               |
|                                                      |
| main            -> 0x08000078                        |
| Reset_Handler   -> 0x08000008                        |
| counter         -> 0x20000000                        |
| _sdata          -> 0x20000000                        |
|                                                      |
+------------------------------------------------------+
|                                                      |
| String Tables                                        |
|                                                      |
| "main"                                               |
| "counter"                                            |
| ".text"                                              |
| ".data"                                              |
|                                                      |
+------------------------------------------------------+
|                                                      |
| Section Header Table                                 |
|                                                      |
| .text                                                |
|   Runtime Addr = 0x08000008                          |
|   File Offset  = 0x1008                              |
|   Size         = 0xc0                                |
|                                                      |
| .data                                                |
|   Runtime Addr = 0x20000000                          |
|   File Offset  = 0x2000                              |
|   Size         = 0x4                                 |
|                                                      |
+------------------------------------------------------+
```

Important:

Section Header Table stores:

- section names
- runtime addresses
- file offsets
- sizes

Symbol Table stores:

- symbol names
- runtime addresses
- owning section

The symbol table does NOT store file offsets.

Those are derived using the section header information.

---

# Common Misconceptions I Had

❌ startup.s defines my variables

✅ startup.s initializes memory regions.

---

❌ linker invents section sizes

✅ object files contribute section sizes.

---

❌ linker invents _edata

✅ linker derives _edata while laying out memory because the linker script requested it.

---

❌ _sdata is in Flash

✅ _sidata is in Flash.

_sdata is in RAM.

---

❌ File offsets belong to symbols.

✅ File offsets belong to sections.

A symbol's file location can be computed from its containing section.

---

# Interview Questions I Should Be Able To Answer

- Walk from main.c to running firmware.
- Explain compiler vs assembler vs linker.
- Explain startup.s.
- Explain linker.ld.
- Explain Reset_Handler.
- Explain .text/.data/.bss.
- Why does .data exist twice?
- Why isn't .bss stored in Flash?
- What is a symbol?
- What are linker symbols?
- Explain runtime address vs file offset.
- Explain how _edata is determined.

---

# Biggest Lessons Learned

The biggest conceptual breakthroughs from this stage were:

1. The linker does not invent section sizes.
2. startup.s works with memory regions rather than variables.
3. The linker script requests linker symbols, and the linker creates them.
4. The ELF is a structured container, not just machine code.
5. File offsets and runtime addresses are two different coordinate systems.
6. The firmware boot process is now understandable from source code all the way to `main()`.

---

# Graduation

I consider Stage 2 complete when I can confidently explain:

```
main.c

↓

compiler

↓

main.o

↓

linker

↓

firmware.elf

↓

firmware.bin

↓

Flash

↓

Vector Table

↓

Reset_Handler

↓

main()
```

without looking anything up.

---

# Next Stage

Stage 3

**How does a C statement change a physical voltage on a GPIO pin?**

Topics:

- MCU memory map
- Memory-mapped I/O
- Peripheral registers
- volatile
- Read-modify-write
- GPIO
- Interrupts