OUTPUT_ARCH(riscv)
ENTRY(_start)

BASE_ADDRESS = DEFINED(BASE_ADDRESS) ? BASE_ADDRESS : 0x80400000;

SECTIONS
{
    . = BASE_ADDRESS;

    .text : {
        *(.text.entry)
        *(.text .text.*)
    }

    . = ALIGN(4K);
    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    }

    . = ALIGN(4K);
    .data : {
        *(.data .data.*)
        *(.sdata .sdata.*)
    }

    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        __bss_end = .;
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.debug*)
    }
}
