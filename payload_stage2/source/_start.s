.section .text.start
.align 4
.arm
.global _start
_start:
_init:

    @ Set up GOT
    sub r3, pc, #8
    ldr r0, =__got_start
    ldr r1, =__got_end

    add r0, r0, r3
    add r1, r1, r3

    got_init:
    cmp r0, r1
    beq gotinit_done
    ldr r2, [r0]
    add r2, r2, r3
    str r2, [r0], #4
    b got_init
    gotinit_done:

    ldr r0, =main
    add r0, r0, r3
    blx r0

.die:
    b .die
