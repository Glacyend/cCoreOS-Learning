/**
 * @file lib.h
 * @brief Minimal user-space C library.
 */

#ifndef __USER_LIB_H_
#define __USER_LIB_H_

#include "syscall.h"

isize write(int fd, const char* buf, usize len);
[[noreturn]] void exit(int code);
isize yield();

#endif /* __USER_LIB_H_ */
