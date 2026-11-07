/**
 * @file syscall.h
 * @brief System call numbers and dispatcher.
 */

#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../config.h"

#define SYSCALL_WRITE 64
#define SYSCALL_EXIT 93
#define SYSCALL_YIELD 124
#define SYSCALL_GET_TIME 169
#define SYSCALL_SBRK 214

/**
 * @brief Dispatch a syscall.
 *
 * @param id   Syscall number (`a7`).
 * @param arg0 First argument (`a0`).
 * @param arg1 Second argument (`a1`).
 * @param arg2 Third argument (`a2`).
 * @return Value to be written back into `a0`.
 */
isize syscall_dispatch(usize id, usize arg0, usize arg1, usize arg2);

#endif /* __SYSCALL_H_ */
