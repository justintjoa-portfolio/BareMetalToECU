# Stage 1 — Cortex-M Boot Process

## Stage Objective

Understand exactly what happens between pressing the power button on an STM32 and reaching `main()`.

The focus of this stage was not peripherals or operating systems, but understanding the boot architecture of a Cortex-M microcontroller.

---

# What I Built

I built a simplified simulator containing:

* ST-Link
* Flash
* RAM
* Memory Map (address decoder)
* CPU
* Firmware
* UART

Each class corresponds to either a real hardware component or a real firmware component.

---

# Complete Boot Sequence

1. Firmware is compiled and linked.
2. The linker creates a firmware image.
3. The firmware image contains:

   * Vector Table
   * Reset_Handler
   * main()
   * Initial .data values
4. ST-Link programs the firmware image into Flash.
5. Power is applied to the STM32.
6. Reset circuitry waits until voltage is stable.
7. CPU reset logic begins.
8. CPU reads vector table entry 0.
9. CPU loads SP.
10. CPU reads vector table entry 1.
11. CPU loads PC.
12. CPU begins executing Reset_Handler.
13. Reset_Handler copies .data into RAM.
14. Reset_Handler zeros .bss.
15. Reset_Handler calls main().
16. main() begins executing the firmware application.

---

# Firmware Image Layout

Flash

## 0x08000000

Vector Table

Entry 0 -> Initial Stack Pointer

Entry 1 -> Reset_Handler address

---

0x08000100

Reset_Handler

---

0x08000200

Initial .data values

---

main()

UART driver

Other firmware

---

# Important Concepts Learned

## What is firmware?

Firmware is simply machine instructions and data stored in Flash.

Firmware does not "run itself."

The CPU executes the firmware.

---

## What is Flash?

Flash is non-volatile memory.

It keeps the firmware even after power is removed.

---

## What is RAM?

RAM is volatile memory.

It loses its contents when power is removed.

The stack, heap, .data, and .bss live in RAM.

---

## What is the vector table?

The vector table is the first structure inside the firmware image.

It contains addresses.

The CPU only uses the first two entries during boot.

Entry 0

Initial Stack Pointer

Entry 1

Reset_Handler address

The remaining entries contain interrupt handler addresses.

---

## What is the Stack Pointer?

The Stack Pointer (SP) is a CPU register.

It points to the current top of the stack in RAM.

---

## What is the Program Counter?

The Program Counter (PC) is a CPU register.

It contains the address of the next instruction to execute.

---

## Who loads SP and PC?

The CPU reset hardware.

Not the firmware.

Not ST-Link.

The CPU itself performs:

SP ← vector_table[0]

PC ← vector_table[1]

---

## Who executes Reset_Handler?

The CPU.

Reset_Handler is simply the first machine code the CPU executes.

---

## Who executes main()?

Reset_Handler.

Reset_Handler contains a normal function call (branch) to main().

The CPU follows that branch instruction.

---

## Does main() return?

Normally no.

Embedded firmware usually executes forever.

Typical embedded firmware:

int main() {

```
while (true) {

}
```

}

If main() accidentally returns, startup code usually enters an infinite loop.

---

## What does ST-Link do?

ST-Link is an external programmer/debugger.

It:

* receives the firmware image
* programs Flash
* disconnects

It never executes firmware.

---

## What does the CPU know?

The CPU does NOT know:

* Flash
* Firmware
* main()
* Reset_Handler

The CPU only knows:

* addresses
* instructions

---

## What does MemoryMap do?

MemoryMap models the STM32 address decoding hardware.

Its job is:

Receive an address.

Determine which hardware owns that address.

Route the read or write.

It also models the Cortex-M boot alias:

CPU requests

0x00000000

↓

MemoryMap

↓

Flash

0x08000000

---

# Biggest Realization

Firmware is passive.

The CPU is active.

The firmware image simply sits inside Flash.

The CPU gives those bytes meaning by fetching and executing them.

---

# Questions I Should Be Able To Answer

## Q1

Where does firmware live?

Answer:

Flash.

---

## Q2

Who executes firmware?

Answer:

The CPU.

---

## Q3

Who loads SP?

Answer:

CPU reset hardware.

---

## Q4

Who loads PC?

Answer:

CPU reset hardware.

---

## Q5

Where does the CPU get those values?

Answer:

Vector table entries 0 and 1.

---

## Q6

Who creates the vector table?

Answer:

The linker.

---

## Q7

Who programs the vector table into Flash?

Answer:

ST-Link.

---

## Q8

Does ST-Link execute firmware?

Answer:

No.

---

## Q9

Why does the CPU read 0x00000000 instead of 0x08000000?

Answer:

Because Cortex-M defines 0x00000000 as the reset address.

STM32 memory mapping hardware aliases that address to Flash.

---

## Q10

What is Reset_Handler?

Answer:

The first firmware function executed after reset.

---

## Q11

Why doesn't the CPU "know" main()?

Answer:

The CPU only understands addresses and instructions.

Reset_Handler branches to main().

---

## Q12

What does CopyDataSection() do?

Answer:

Copies initialized global variables from Flash into RAM.

---

## Q13

What does ZeroBssSection() do?

Answer:

Zeros uninitialized global variables in RAM.

---

## Q14

Why does firmware copy .data into RAM?

Answer:

Because writable globals cannot remain in Flash.

---

## Q15

What is the biggest architectural lesson?

Answer:

The CPU, firmware, Flash, RAM, ST-Link, and MemoryMap all have distinct responsibilities.

Understanding those responsibilities is the foundation for understanding embedded systems.
