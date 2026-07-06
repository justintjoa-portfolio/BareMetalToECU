# BareMetalToECU Roadmap

## Phase 1: Bare-metal foundations

### Stage 0: Birth / Boot
CPU reset, vector table, initial SP, PC, Reset_Handler, and first control flow.

### Stage 1: Firmware image and flashing model
ST-Link concept, Flash image, vector table placement, linker output, ELF/BIN idea.

### Stage 2A: Runtime memory model
Flash vs RAM, .text, .rodata, .data, .bss, stack, linker symbols.

### Stage 2B: Real STM32 bare-metal blink
Custom startup code, custom linker script, no HAL, no Cube-generated code, real GPIO register writes.

## Phase 2: Peripheral foundations

### Stage 3: GPIO driver model
Shared simulator/STM32 GPIO abstraction, register-level driver shape, volatile and MMIO.

### Stage 4: UART
Polling UART, TX register, status register, simulator UART vs real UART.

### Stage 5: Time
Busy delay, SysTick, timer peripheral, periodic tasks.

### Stage 6: Interrupts
Vector table expansion, NVIC, ISR flow, context saving basics.

## Phase 3: Toward an ECU

### Stage 7: Superloop architecture
Task decomposition without RTOS.

### Stage 8: Cooperative scheduler
Tasks, periods, run-to-completion semantics.

### Stage 9: Execution contexts
Stacks, context switching model, process vs thread foundations.

### Stage 10: RTOS concepts
Preemption, mutexes, semaphores, queues, priority inversion.

### Stage 11: Tiny ECU
Sensors, actuator output, logging, watchdog, fault handling.
