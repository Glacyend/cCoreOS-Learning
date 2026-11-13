/**
 * @file syscall.h
 * @brief Implementation of syscalls.
 *
 * The single entry point to all system calls, `syscall_dispatch()`, is called
 * whenever userspace wishes to perform a system call using the `ecall`
 * instruction. In this case, the processor raises an "Environment call from
 * U-mode" exception, which is handled as one of the cases in `trap_handler()`.
 *
 * For clarity, each single syscall is implemented as its own function, named
 * `sys_` then the name of the syscall. You can find functions like this in
 * submodules, and you should also implement syscalls this way.
 */

#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../config.h"

/** @brief Syscall number for `write`. */
#define SYSCALL_WRITE 64

/** @brief Syscall number for `exit`. */
#define SYSCALL_EXIT 93

/** @brief Syscall number for `yield`. */
#define SYSCALL_YIELD 124

/** @brief Syscall number for `get_time`. */
#define SYSCALL_GET_TIME 169

/** @brief Syscall number for `sbrk`. */
#define SYSCALL_SBRK 214

/**
 * @brief Handle syscall exception with `syscall_id` and other arguments.
 *
 * Rust equivalent: `syscall`.
 *
 * @param syscall_id Syscall number (`a7`).
 * @param arg0       First argument (`a0`).
 * @param arg1       Second argument (`a1`).
 * @param arg2       Third argument (`a2`).
 * @return Value to be written back into `a0`.
 */
isize syscall_dispatch(usize syscall_id, usize arg0, usize arg1, usize arg2);

#endif /* __SYSCALL_H_ */
