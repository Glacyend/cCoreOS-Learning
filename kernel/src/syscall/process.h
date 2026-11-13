/**
 * @file process.h
 * @brief Process management syscalls.
 */

#ifndef __SYSCALL_PROCESS_H_
#define __SYSCALL_PROCESS_H_

#include "../config.h"

/**
 * @brief Task exits and submit an exit code.
 *
 * Rust equivalent: `sys_exit`.
 *
 * @param exit_code Exit code reported in the log.
 */
[[noreturn]] void sys_exit(int32_t exit_code);

/**
 * @brief Current task gives up resources for other tasks.
 *
 * Rust equivalent: `sys_yield`.
 */
isize sys_yield();

/**
 * @brief Get current time.
 *
 * Rust equivalent: `sys_get_time`.
 */
isize sys_get_time();

/**
 * @brief Change data segment size.
 *
 * Rust equivalent: `sys_sbrk`.
 *
 * @param size Signed delta for the program break.
 * @return The old program break on success, or -1 on failure.
 */
isize sys_sbrk(int32_t size);

#endif /* __SYSCALL_PROCESS_H_ */
