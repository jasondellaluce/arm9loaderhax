.arm
.global flush_all_caches
.type   flush_all_caches STT_FUNC

@void flush_all_caches(void)
flush_all_caches:
	push {lr}
	@ Flush caches
	ldr r0, =0xFFFF0830 @ Nintendo's flush function in unprot. bootrom
	blx r0

	@ flush instruction cache, it's not flushed by Nintendo's function
	mov r0, #0
	mcr p15, 0, r0, c7, c5, 0

	pop {lr}
	bx lr

