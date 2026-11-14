/**
 * @file syscall.c
 * @brief User-side syscall wrappers.
 */

#include "syscall.h"

/**
 * @brief Issue an `ecall` with the given syscall number and arguments.
 *
 * Rust equivalent: `syscall`. `a7` holds the syscall number, `a0` / `a1` /
 * `a2` hold the first three arguments, and `a0` receives the return value.
 */
static long syscall(size_t syscall_id, size_t arg0, size_t arg1, size_t arg2) {
    register size_t x10 __asm__("a0") = arg0;
    register size_t x11 __asm__("a1") = arg1;
    register size_t x12 __asm__("a2") = arg2;
    register size_t x17 __asm__("a7") = syscall_id;
    __asm__ volatile("ecall" : "+r"(x10) : "r"(x11), "r"(x12), "r"(x17) : "memory");
    return (long)x10;
}

long sys_write(size_t fd, const char* buf, size_t len) {
    return syscall(SYSCALL_WRITE, fd, (size_t)buf, len);
}

[[noreturn]] void sys_exit(int exit_code) {
    syscall(SYSCALL_EXIT, (size_t)exit_code, 0, 0);
    __builtin_unreachable();
}

long sys_yield() {
    return syscall(SYSCALL_YIELD, 0, 0, 0);
}

long sys_get_time() {
    return syscall(SYSCALL_GET_TIME, 0, 0, 0);
}

long sys_sbrk(int size) {
    return syscall(SYSCALL_SBRK, (size_t)size, 0, 0);
}
