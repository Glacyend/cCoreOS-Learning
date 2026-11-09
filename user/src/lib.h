/**
 * @file lib.h
 * @brief Minimal user-space C library.
 */

#ifndef __USER_LIB_H_
#define __USER_LIB_H_

#include "syscall.h"

isize user_write(int fd, const char* buf, usize len);
[[noreturn]] void user_exit(int code);
isize user_yield();
isize user_get_time();

#endif /* __USER_LIB_H_ */
