/**
 * @file syscall.c
 * @brief User-side syscall wrappers.
 */

#include "syscall.h"

static inline isize syscall(usize id, usize a0, usize a1, usize a2) {
    register usize x10 __asm__("a0") = a0;
    register usize x11 __asm__("a1") = a1;
    register usize x12 __asm__("a2") = a2;
    register usize x17 __asm__("a7") = id;
    __asm__ volatile("ecall" : "+r"(x10) : "r"(x11), "r"(x12), "r"(x17) : "memory");
    return (isize)x10;
}

isize sys_write(usize fd, const char* buf, usize len) {
    return syscall(SYSCALL_WRITE, fd, (usize)buf, len);
}

[[noreturn]] void sys_exit(int code) {
    syscall(SYSCALL_EXIT, (usize)code, 0, 0);
    __builtin_unreachable();
}

isize sys_yield() {
    return syscall(SYSCALL_YIELD, 0, 0, 0);
}
