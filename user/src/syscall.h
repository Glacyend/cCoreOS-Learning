/**
 * @file syscall.h
 * @brief User-side syscall wrappers.
 */

#ifndef __USER_SYSCALL_H_
#define __USER_SYSCALL_H_

#include <stddef.h>
#include <stdint.h>

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
 * @brief Write @p len bytes from @p buf to file descriptor `fd`.
 *
 * Rust equivalent: `sys_write`. Rust takes `&[u8]`; C takes
 * `(const char*, size_t)` because it has no slices.
 */
long sys_write(size_t fd, const char* buf, size_t len);

/**
 * @brief Terminate the current task.
 *
 * Rust equivalent: `sys_exit`. The Rust signature is `-> long`, but the
 * kernel never returns to the caller; C marks the function `[[noreturn]]` to
 * express the same intent.
 */
[[noreturn]] void sys_exit(int exit_code);

/**
 * @brief Yield the CPU to the next Ready task.
 *
 * Rust equivalent: `sys_yield`.
 */
long sys_yield();

/**
 * @brief Get current time in milliseconds.
 *
 * Rust equivalent: `sys_get_time`.
 */
long sys_get_time();

/**
 * @brief Change the data segment size.
 *
 * Rust equivalent: `sys_sbrk`.
 *
 * @param size Signed delta for the program break.
 * @return The old program break on success, or -1 on failure.
 */
long sys_sbrk(int size);

#endif /* __USER_SYSCALL_H_ */
