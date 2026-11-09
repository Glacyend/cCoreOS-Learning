    .align 3
    .section .data
    .globl _num_app
_num_app:
    .quad 4
    .quad app_00_sleep_3_start
    .quad app_01_sleep_5_start
    .quad app_02_bubble_sort_start
    .quad app_03_power_13_start
    .quad app_03_power_13_end

    .section .data
    .globl app_00_sleep_3_start
    .globl app_00_sleep_3_end
app_00_sleep_3_start:
    .incbin "../user/build/bin/00_sleep_3.bin"
app_00_sleep_3_end:

    .section .data
    .globl app_01_sleep_5_start
    .globl app_01_sleep_5_end
app_01_sleep_5_start:
    .incbin "../user/build/bin/01_sleep_5.bin"
app_01_sleep_5_end:

    .section .data
    .globl app_02_bubble_sort_start
    .globl app_02_bubble_sort_end
app_02_bubble_sort_start:
    .incbin "../user/build/bin/02_bubble_sort.bin"
app_02_bubble_sort_end:

    .section .data
    .globl app_03_power_13_start
    .globl app_03_power_13_end
app_03_power_13_start:
    .incbin "../user/build/bin/03_power_13.bin"
app_03_power_13_end:

