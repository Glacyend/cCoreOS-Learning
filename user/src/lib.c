/**
 * @file lib.c
 * @brief Minimal user-space C library.
 */

#include "lib.h"

isize user_write(int fd, const char* buf, usize len) {
    return sys_write((usize)fd, buf, len);
}

[[noreturn]] void user_exit(int code) {
    sys_exit(code);
}

isize user_yield() {
    return sys_yield();
}

isize user_get_time() {
    return sys_get_time();
}
