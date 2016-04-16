.section .text.start
.align 4
.arm
.global _start
_start:
	b _init
	nop
	nop
_init:
    @ Change the stack pointer
    mov sp, #0x27000000
	
	msr cpsr_c, #0xDF
    ldr r0, =0x10000035
    MRC     p15, 0, R0,c2,c0, 0
	MRC     p15, 0, R12,c2,c0, 1
	MRC     p15, 0, R1,c3,c0, 0
	MRC     p15, 0, R2,c5,c0, 2
	MRC     p15, 0, R3,c5,c0, 3
	LDR     R4, =0x18000035
	BIC     R2, R2, #0xF0000
	BIC     R3, R3, #0xF0000
	ORR     R0, R0, #0x10
	ORR     R2, R2, #0x30000
	ORR     R3, R3, #0x30000
	ORR     R12, R12, #0x10
	ORR     R1, R1, #0x10
	MCR     p15, 0, R0,c2,c0, 0
	MCR     p15, 0, R12,c2,c0, 1
	MCR     p15, 0, R1,c3,c0, 0
	MCR     p15, 0, R2,c5,c0, 2
	MCR     p15, 0, R3,c5,c0, 3
	MCR     p15, 0, R4,c6,c4, 0
	MRC     p15, 0, R0,c2,c0, 0
	MRC     p15, 0, R1,c2,c0, 1
	MRC     p15, 0, R2,c3,c0, 0
	ORR     R0, R0, #0x20
	ORR     R1, R1, #0x20
	ORR     R2, R2, #0x20
	MCR     p15, 0, R0,c2,c0, 0
	MCR     p15, 0, R1,c2,c0, 1
	MCR     p15, 0, R2,c3,c0, 0

    @ Undocumented: Fixes mounting of SDMC
    ldr r0, =0x10000020
    mov r1, #0x340
    str r1, [r0]

    bl main

.die:
    b .die