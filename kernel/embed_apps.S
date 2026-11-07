    .align 3
    .section .data
    .globl _num_app
_num_app:
    .quad 1
    .quad app_00_hello_start
    .quad app_00_hello_end

    .section .data
    .globl app_00_hello_start
    .globl app_00_hello_end
app_00_hello_start:
    .incbin "../user/build/bin/00_hello.bin"
app_00_hello_end:

