.section .text.start
.align 4
.global _start
.extern flush_all_caches

_start:
    @ Disable IRQ
    mrs r0, cpsr
    orr r0, r0, #0x80
    msr cpsr_c, r0

    @ Change the stack pointer
    mov sp, #0x27000000

    @ Set up relocation register (r9)
    @   this instruction is 16 bytes away from the start, takes into account the 8
    @   bytes PC is ahead, meaning pc - 24 == start in memory
    sub r9, pc, #24

    @ Disable caches and MPU
    ldr r0, =disable_mpu_and_caching
    add r0, r0, r9
    blx r0

    @ Flush caches
    ldr r0, =flush_all_caches
    add r0, r0, r9
    blx r0

    @ Setup GOT table (just in case)
    ldr r0, =__got_start
    ldr r1, =__got_end
    mov r2, r9

    ldr r3, =relocate_section
    add r3, r3, r9
    blx r3

    @ Set MPU table
    ldr r0, =setup_mpu_table
    add r0, r0, r9
    blx r0

    @ Enable caches and MPU
    ldr r0, =enable_mpu_and_caching
    add r0, r0, r9
    blx r0

    ldr r0, =main
    add r0, r0, r9
    blx r0

.die:
    b .die

disable_mpu_and_caching:
    @ Disable caches and MPU
    mrc p15, 0, r0, c1, c0, 0  @ read control register
    bic r0, r0, #(1<<12)       @ - instruction cache enable
    bic r0, r0, #(1<<2)        @ - data cache enable
    bic r0, r0, #(1<<0)        @ - mpu enable
    mcr p15, 0, r0, c1, c0, 0  @ write control register

    bx lr

enable_mpu_and_caching:
    @ Enable caches and MPU
    mrc p15, 0, r0, c1, c0, 0  @ read control register
    orr r0, r0, #(1<<12)       @ - instruction cache enable
    orr r0, r0, #(1<<2)        @ - data cache enable
    orr r0, r0, #(1<<0)        @ - mpu enable
    mcr p15, 0, r0, c1, c0, 0  @ write control register

    bx lr

setup_mpu_table:
    push {r4-r7}
    @ Set MPU permissions and cache settings
    ldr r0, =0xFFFF001D @ ffff0000 32k
    ldr r1, =0x01FF801D @ 01ff8000 32k
    ldr r2, =0x08000027 @ 08000000 1M
    ldr r3, =0x10000021 @ 10000000 128k
    ldr r4, =0x10100025 @ 10100000 512k
    ldr r5, =0x20000035 @ 20000000 128M
    ldr r6, =0x1FF00027 @ 1FF00000 1M
    ldr r7, =0x1800002D @ 18000000 8M
    mcr p15, 0, r0, c6, c0, 0
    mcr p15, 0, r1, c6, c1, 0
    mcr p15, 0, r2, c6, c2, 0
    mcr p15, 0, r3, c6, c3, 0
    mcr p15, 0, r4, c6, c4, 0
    mcr p15, 0, r5, c6, c5, 0
    mcr p15, 0, r6, c6, c6, 0
    mcr p15, 0, r7, c6, c7, 0
    mov r0, #0x25
    mcr p15, 0, r0, c2, c0, 0  @ data cacheable
    mcr p15, 0, r0, c2, c0, 1  @ instruction cacheable
    mcr p15, 0, r0, c3, c0, 0  @ data bufferable

    @ Give read/write access to all the memory regions
    ldr r0, =0x33333333
    mcr p15, 0, r0, c5, c0, 2 @ write data access
    mcr p15, 0, r0, c5, c0, 3 @ write instruction access

    pop {r4-r7}
    bx lr

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

