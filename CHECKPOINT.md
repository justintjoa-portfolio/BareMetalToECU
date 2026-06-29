# BareMetalToECU Checkpoint

## Current status

**Stage 2B complete:** custom bare-metal STM32F446 firmware successfully flashes and blinks the onboard LED using your own startup code, linker script, reset handler, and register writes.

## Confirmed working path

```text
power/reset
-> vector table at 0x08000000
-> initial SP loaded from vector_table[0]
-> Reset_Handler loaded from vector_table[1]
-> Reset_Handler copies .data
-> Reset_Handler zeros .bss
-> Reset_Handler calls main
-> main enables GPIOA clock
-> main configures PA5 as output
-> main toggles PA5 using BSRR
-> LED blinks
```

## Important bug fixed

The reset handler must be marked as a Thumb function in `startup.s`:

```asm
.section .text.Reset_Handler
.thumb_func
Reset_Handler:
```

Without `.thumb_func`, the vector table may contain an even reset address. Cortex-M executes Thumb code, so exception vectors must resolve to a Thumb entry address with bit 0 set.

## Stage 2B goal going forward

Keep the simulator and real STM32 firmware as close to 1:1 as practical.

Both should model this same logical firmware script:

```text
ResetHandler
  CopyDataSection
  ZeroBssSection
  Main

Main
  EnableGpioAClock
  ConfigurePa5AsOutput
  forever:
    SetPa5High
    Delay
    SetPa5Low
    Delay
```

## Current hardware target

STM32F446-style Nucleo board using PA5 onboard LED.

Key STM32F446 addresses used now:

```text
RCC base        0x40023800
RCC_AHB1ENR     0x40023830
GPIOA base      0x40020000
GPIOA_MODER     0x40020000
GPIOA_ODR       0x40020014
GPIOA_BSRR      0x40020018
PA5 mask        1 << 5
```

## Next step

Refactor the simulator and STM32 blink firmware to use matching function names and matching register concepts.
