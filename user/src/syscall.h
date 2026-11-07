/**
 * @file syscall.h
 * @brief User-side syscall wrappers.
 */

#ifndef __USER_SYSCALL_H_
#define __USER_SYSCALL_H_

typedef unsigned long usize;
typedef long isize;

#define SYSCALL_WRITE 64
#define SYSCALL_EXIT 93
#define SYSCALL_YIELD 124
#define SYSCALL_GET_TIME 169
#define SYSCALL_SBRK 214

isize sys_write(usize fd, const char* buf, usize len);
[[noreturn]] void sys_exit(int code);
isize sys_yield();

#endif /* __USER_SYSCALL_H_ */
