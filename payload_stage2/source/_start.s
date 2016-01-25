.section .text.start
.align 4
.arm
.global _start
_start:
_init:

    bl main

.die:
    b .die