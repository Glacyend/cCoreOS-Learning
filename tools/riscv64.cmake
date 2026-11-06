set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET riscv64-unknown-elf)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_COMPILER_TARGET riscv64-unknown-elf)

set(CMAKE_LINKER ld.lld)
set(CMAKE_OBJCOPY llvm-objcopy)
set(CMAKE_OBJDUMP llvm-objdump)

set(CMAKE_C_LINKER_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_INIT "-march=rv64gc -mabi=lp64d -mcmodel=medany -ffreestanding -nostdlib -fno-stack-protector -fno-builtin -fno-common -Wall -Wextra")
set(CMAKE_ASM_FLAGS_INIT "-march=rv64gc -mabi=lp64d -mcmodel=medany")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-mno-relax -nostdlib -static -Wl,--build-id=none -fuse-ld=lld")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
