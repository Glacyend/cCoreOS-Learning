    .section .text.entry
    .globl _start
_start:
    // clear .bss
    la t0, __bss_start
    la t1, __bss_end
1:
    bgeu t0, t1, 2f
    sb zero, 0(t0)
    addi t0, t0, 1
    j 1b
2:
    call main
    li a7, 93
    ecall
