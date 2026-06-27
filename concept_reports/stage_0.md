# Stage 0 - Birth of a Computer

## Objective

The objective of Stage 0 is to understand what happens from the moment power is applied to a bare-metal microcontroller until the firmware reaches `main()`.

Unlike desktop applications, there is **no operating system** responsible for preparing the program before execution. The firmware itself must perform all initialization before `main()` is called.

---

# What We Built

We built a simplified simulation of the Cortex-M boot sequence.

Our simulator performs the following sequence:

```
Power Applied
        │
        ▼
CPU comes out of reset
        │
        ▼
CPU reads the Vector Table from Flash
        │
        ├── Entry 0 → Initial Stack Pointer (SP)
        │
        └── Entry 1 → Reset_Handler address (PC)
        │
        ▼
CPU begins executing Reset_Handler
        │
        ▼
Reset_Handler calls main()
        │
        ▼
UART prints "Hello World"
```

Although simplified, this models the high-level startup sequence of a real Cortex-M processor.

---

# Hardware Overview

Our conceptual STM32 contains:

```
+------------------------------------------------------+

                STM32

--------------------------------------------------------

CPU (Cortex-M)

Flash Memory
    • Non-volatile
    • Stores firmware

RAM
    • Volatile
    • Stores writable program state

UART

GPIO

Timers

Interrupt Controller

+------------------------------------------------------+
```

---

# What is Firmware?

Firmware is the first software executed after power-on.

It gives otherwise "dead" silicon a purpose.

Its responsibilities include:

- preparing the runtime
- initializing hardware
- configuring peripherals
- eventually calling `main()`

In Stage 0 we only model the beginning of this process.

---

# What is Flash?

Flash is non-volatile semiconductor memory.

Unlike RAM, Flash retains its contents after power is removed.

Think of Flash as permanent storage built directly into the microcontroller.

The firmware image is programmed ("flashed") into Flash before the processor is powered on.

---

# Where Does Firmware Live?

The compiled firmware resides inside Flash.

Conceptually:

```
Flash

+-------------------------------------+
| Vector Table                        |
+-------------------------------------+
| .text                               |
| Reset_Handler                       |
| main()                              |
| UART driver                         |
+-------------------------------------+
| .rodata                             |
| "Hello World"                       |
+-------------------------------------+
| Initial .data values                |
+-------------------------------------+
```

---

# What is the Vector Table?

The Vector Table is a table stored at the beginning of Flash.

It contains addresses used by the CPU during startup and when handling interrupts.

Conceptually:

```
Flash

0x00000000

+-------------------------------------+
| Entry 0                             |
| Initial Stack Pointer               |
+-------------------------------------+
| Entry 1                             |
| Reset_Handler                       |
+-------------------------------------+
| Entry 2                             |
| NMI_Handler                         |
+-------------------------------------+
| Entry 3                             |
| HardFault_Handler                   |
+-------------------------------------+
| ...                                 |
+-------------------------------------+
```

The CPU **does not modify** the Vector Table.

It only reads it.

---

# Are the First Two Entries Hardcoded?

For a given firmware image, yes.

The first two entries remain constant while the firmware is executing.

However, they are **generated during the build process**.

The linker determines:

- where `Reset_Handler` is located
- where the initial stack begins

and writes those addresses into the Vector Table before the firmware is flashed.

If the firmware changes, the linker automatically regenerates the Vector Table.

---

# Stack Pointer (SP)

The Stack Pointer points to the top of the stack in RAM.

The stack stores:

- local variables
- return addresses
- saved registers
- function call frames

Initially:

```
SP = vector_table[0]
```

---

# Program Counter (PC)

The Program Counter stores the address of the next instruction to execute.

Initially:

```
PC = vector_table[1]
```

After reset, execution begins at `Reset_Handler`.

---

# Why Doesn't the CPU Call main()?

The CPU has no knowledge of C or C++.

It does not know what `main()` is.

Instead, the CPU simply:

1. Reads the initial Stack Pointer.
2. Reads the address of `Reset_Handler`.
3. Begins executing instructions there.

`Reset_Handler` prepares the runtime before eventually calling `main()`.

---

# What Is Fake About Our Simulation?

Our simulation intentionally simplifies several pieces of hardware.

Instead of real Cortex-M hardware:

- the Vector Table is a C++ struct
- SP and PC are C++ variables
- `PowerOn()` directly invokes `ResetHandler()`
- UART prints to `std::cout`
- no instruction fetch/decode/execute occurs
- `.data` copy and `.bss` zeroing are skipped

These simplifications allow us to focus on understanding the architecture before adding complexity.

---

# What Is Accurate?

Despite the simplifications, the following concepts accurately model a real Cortex-M system:

- firmware resides in Flash
- the Vector Table resides in Flash
- the CPU reads the Vector Table after reset
- execution begins at `Reset_Handler`
- `Reset_Handler` prepares the runtime
- `Reset_Handler` calls `main()`
- `main()` is **not** the first code executed

---

# Reflection Questions

## 1. Why does `Reset_Handler` call `main()` instead of the CPU?

**Answer**

The CPU has no concept of `main()`.

It only knows how to load the initial Stack Pointer and Program Counter from the Vector Table.

The Program Counter points to `Reset_Handler`, which prepares the runtime before calling `main()`.

---

## 2. What are SP and PC?

**Answer**

SP (Stack Pointer) points to the top of the program stack.

PC (Program Counter) points to the next instruction the CPU will execute.

Immediately after reset:

```
SP = vector_table[0]

PC = vector_table[1]
```

---

## 3. What are `vector_table[0]` and `vector_table[1]`?

**Answer**

`vector_table[0]`

The initial Stack Pointer.

`vector_table[1]`

The address of `Reset_Handler`.

---

## 4. What is fake about this simulation?

**Answer**

The simulator does not emulate Cortex-M instructions.

Instead, it manually models the startup sequence using ordinary C++ objects and function calls.

---

## 5. What is conceptually accurate?

**Answer**

The simulator correctly demonstrates:

- firmware stored in Flash
- Vector Table lookup
- loading SP
- loading PC
- execution beginning at `Reset_Handler`
- `Reset_Handler` eventually calling `main()`

---

## 6. What will future stages add?

Future stages will implement the work performed by a real `Reset_Handler`.

Specifically:

- copy `.data` from Flash into RAM
- zero `.bss`
- initialize global objects
- configure clocks
- initialize peripherals

---

# Stage 0 Summary

At the completion of Stage 0 I understand:

- what firmware is
- why firmware lives in Flash
- what Flash is
- why RAM is different from Flash
- what the Vector Table is
- where the Vector Table lives
- why the CPU begins execution at `Reset_Handler`
- why `main()` is not the first function executed
- the purpose of the Stack Pointer
- the purpose of the Program Counter
- the high-level boot sequence of a Cortex-M processor

---

# What We Don't Understand Yet

Questions intentionally left unanswered for future stages:

- How does the linker generate the Vector Table?
- How are addresses assigned during linking?
- How does the CPU fetch and decode instructions?
- Why must `.data` be copied into RAM?
- Why must `.bss` be zeroed?
- How are interrupts dispatched?
- How does the stack actually grow?
- How are heap and stack created?
- How does the CPU execute machine instructions?