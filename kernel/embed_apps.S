    .align 3
    .section .data
    .globl _num_app
_num_app:
    .quad 4
    .quad app_00_hello_start
    .quad app_01_power_7_start
    .quad app_02_power_5_start
    .quad app_03_power_13_start
    .quad app_03_power_13_end

    .section .data
    .globl app_00_hello_start
    .globl app_00_hello_end
app_00_hello_start:
    .incbin "../user/build/bin/00_hello.bin"
app_00_hello_end:

    .section .data
    .globl app_01_power_7_start
    .globl app_01_power_7_end
app_01_power_7_start:
    .incbin "../user/build/bin/01_power_7.bin"
app_01_power_7_end:

    .section .data
    .globl app_02_power_5_start
    .globl app_02_power_5_end
app_02_power_5_start:
    .incbin "../user/build/bin/02_power_5.bin"
app_02_power_5_end:

    .section .data
    .globl app_03_power_13_start
    .globl app_03_power_13_end
app_03_power_13_start:
    .incbin "../user/build/bin/03_power_13.bin"
app_03_power_13_end:

