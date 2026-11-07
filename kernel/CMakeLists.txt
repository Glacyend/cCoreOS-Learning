cmake_minimum_required(VERSION 3.31)
project(ccoreos C ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(KERNEL_SOURCES
    src/console.c
    src/device_tree.c
    src/entry.S
    src/logging.c
    src/main.c
    src/panic.c
    src/sbi.c
    src/syscall/fs.c
    src/syscall/process.c
    src/syscall/syscall.c
    src/timer.c
    src/trap/trap.c
    src/trap/trap.S
)

set(KERNEL_ELF ${CMAKE_BINARY_DIR}/kernel.elf)

add_executable(kernel.elf ${KERNEL_SOURCES})

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
)

target_link_options(kernel.elf PRIVATE
    -Wl,-e,_start
)

add_custom_command(TARGET kernel.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:kernel.elf>
            ${CMAKE_BINARY_DIR}/kernel.bin
    COMMENT "objcopy kernel.elf -> kernel.bin"
)
