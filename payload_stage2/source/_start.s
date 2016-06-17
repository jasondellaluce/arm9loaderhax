.section .text.start
.align 4
.arm
.global _start
.extern flush_all_caches
_start:
_init:

    @ Set up GOT
    sub r9, pc, #8
    ldr r0, =__got_start
    ldr r1, =__got_end
    mov r2, r9

    ldr r3, =relocate_section
    add r3, r3, r9
    blx r3

    ldr r0, =main
    add r0, r0, r9
    blx r0

.die:
    b .die

@ r0 - region start
@ r1 - region end
@ r2 - relocation base (usually starting PC address)
relocate_section:
    add r0, r0, r2
    add r1, r1, r2

    .Lreloc_init:
    cmp r0, r1
    beq .Lrelocinit_done
    ldr r3, [r0]
    add r3, r2, r3
    str r3, [r0], #4
    b .Lreloc_init
    .Lrelocinit_done:

    bx lr
