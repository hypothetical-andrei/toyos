.section .text
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0
    bl main

loop:
    wfe
    b loop

.section .bss
.space 4096
stack_top:
