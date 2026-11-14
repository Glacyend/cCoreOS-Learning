/**
 * @file lib.c
 * @brief Minimal user-space C library.
 */

#include "lib.h"
#include "syscall.h"

long user_write(int fd, const char* buf, size_t len) {
    return sys_write((size_t)fd, buf, len);
}

[[noreturn]] void user_exit(int code) {
    sys_exit(code);
}

long user_yield() {
    return sys_yield();
}

long user_get_time() {
    return sys_get_time();
}

long user_sbrk(int size) {
    return sys_sbrk(size);
}
