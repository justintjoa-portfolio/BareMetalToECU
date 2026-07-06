.syntax unified
.cpu cortex-m4
.thumb

.global vector_table
.global Reset_Handler

.extern main
.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.section .isr_vector, "a", %progbits
vector_table:
  .word _estack
  .word Reset_Handler

.section .text.Reset_Handler
.thumb_func
Reset_Handler:
  ldr r0, =_sidata
  ldr r1, =_sdata
  ldr r2, =_edata

CopyData:
  cmp r1, r2
  bcc CopyDataLoop
  b ZeroBssStart

CopyDataLoop:
  ldr r3, [r0], #4
  str r3, [r1], #4
  b CopyData

ZeroBssStart:
  ldr r0, =_sbss
  ldr r1, =_ebss
  movs r2, #0

ZeroBss:
  cmp r0, r1
  bcc ZeroBssLoop
  b CallMain

ZeroBssLoop:
  str r2, [r0], #4
  b ZeroBss

CallMain:
  bl main

Hang:
  b Hang
