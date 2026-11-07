/**
 * @file lib.c
 * @brief Minimal user-space C library.
 */

#include "lib.h"

isize write(int fd, const char* buf, usize len) {
    return sys_write((usize)fd, buf, len);
}

[[noreturn]] void exit(int code) {
    sys_exit(code);
}

isize yield() {
    return sys_yield();
}
