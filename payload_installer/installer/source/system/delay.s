// Copyright 2014 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

.arm
.global ioDelay
.type   ioDelay STT_FUNC

@ioDelay ( u32 us )
ioDelay:
	ldr r1, =0x18000000  @ VRAM
1:
	@ Loop doing uncached reads from VRAM to make loop timing more reliable
	ldr r2, [r1]
	subs r0, #1
	bgt 1b
	bx lr
	
.arm
.global waitcycles
.type   waitcycles STT_FUNC

@waitcycles ( u32 us )
waitcycles:
	PUSH    {R0-R2,LR}
	STR     R0, [SP,#4]
	waitcycles_loop:
		LDR     R3, [SP,#4]
		SUBS    R2, R3, #1
		STR     R2, [SP,#4]
		CMP     R3, #0
		BNE     waitcycles_loop
	POP     {R0-R2,PC}

