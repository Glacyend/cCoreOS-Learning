.altmacro
.macro SAVE_GP n
    sd x\n, \n*8(sp)
.endm
.macro LOAD_GP n
    ld x\n, \n*8(sp)
.endm

    .section .text
    .globl __alltraps
    .globl __restore
    .align 2

__alltraps:
    csrrw sp, sscratch, sp
    // now sp -> kernel stack, sscratch -> user stack
    addi sp, sp, -34*8
    // save general-purpose registers
    sd x1, 1*8(sp)
    sd x3, 3*8(sp)
    .set n, 5
    .rept 27
        SAVE_GP %n
        .set n, n + 1
    .endr
    // save sstatus and sepc
    csrr t0, sstatus
    csrr t1, sepc
    sd t0, 32*8(sp)
    sd t1, 33*8(sp)
    // read user stack from sscratch and save it into x[2]
    csrr t2, sscratch
    sd t2, 2*8(sp)
    // call trap_handler(cx = &TrapContext)
    mv a0, sp
    call trap_handler

__restore:
    // a0 = &TrapContext (in kernel stack)
    mv sp, a0
    ld t0, 32*8(sp)
    ld t1, 33*8(sp)
    ld t2, 2*8(sp)
    csrw sstatus, t0
    csrw sepc, t1
    csrw sscratch, t2
    // restore general-purpose registers except sp/tp
    ld x1, 1*8(sp)
    ld x3, 3*8(sp)
    .set n, 5
    .rept 27
        LOAD_GP %n
        .set n, n + 1
    .endr
    // release TrapContext on kernel stack
    addi sp, sp, 34*8
    csrrw sp, sscratch, sp
    sret
