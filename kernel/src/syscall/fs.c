/**
 * @file fs.c
 * @brief File and filesystem-related syscalls.
 */

#include "fs.h"
#include "../config.h"
#include "../console.h"

#define FD_STDOUT 1

isize sys_write(usize fd, const char* buf, usize len) {
    if (fd != FD_STDOUT) {
        return -1;
    }
    for (usize i = 0; i < len; i++) {
        console_putchar(buf[i]);
    }
    return (isize)len;
}
