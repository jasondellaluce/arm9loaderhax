.arm
.arch armv6k

gpuReg = 		0x10400000
framebuffers =	0x20000000

.global arm11Code
arm11Code:
	arm11CodeStart:	
		ldr r0, =gpuReg
		
		@ Set framebuffers
		ldr r2, =framebuffers
		str r2, [r0, #0x468]
		str	r2, [r0, #0x46C]
		str	r2, [r0, #0x494]
		str	r2, [r0, #0x498]
		ldr r2, =framebuffers + 0x60000
		str	r2, [r0, #0x568]
		str	r2, [r0, #0x56C]
		mov	r1, #0
		str	r1, [r0, #0x478]
		str	r1, [r0, #0x578]
		
		@ Wait for Arm11 entrypoint
		mov r0, #0x1FFFFFF8
		mov r1, #0
		str r1, [r0]
		wait_arm11_loop:
			ldr r1, [r0]
			cmp r1, #0
			beq wait_arm11_loop
		bx r1
		
.pool
	arm11CodeEnd:

.global arm11CodeSize
arm11CodeSize:
	.word arm11CodeEnd-arm11CodeStart
	
.global topScreen
topScreen:
	.word framebuffers

.global subScreen
subScreen:
	.word framebuffers + 0x60000