cmake_minimum_required(VERSION 3.31)
project(user C ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(USER_LIB_SOURCES
    src/console.c
    src/lib.c
    src/panic.c
    src/start.S
    src/syscall.c
)

add_library(user STATIC ${USER_LIB_SOURCES})
target_include_directories(user PUBLIC src)

add_subdirectory(bin)
