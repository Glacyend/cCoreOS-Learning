/**
 * @file lib.h
 * @brief Minimal user-space C library.
 *
 * Rust equivalent: `lib.rs`. The `_start` entry point lives in `start.S`,
 * matching the Rust `_start` defined in `lib.rs`.
 */

#ifndef __USER_LIB_H_
#define __USER_LIB_H_

#include <stddef.h>

/**
 * @brief Write @p len bytes from @p buf to file descriptor @p fd .
 *
 * Rust equivalent: `write`. Rust takes `&[u8]`; C takes
 * `(const char*, size_t)` because it has no slices.
 */
long user_write(int fd, const char* buf, size_t len);

/**
 * @brief Terminate the current task.
 *
 * Rust equivalent: `exit`. The Rust signature is `-> long`, but the kernel
 * never returns to the caller; C marks the function `[[noreturn]]` to express
 * the same intent.
 */
[[noreturn]] void user_exit(int code);

/**
 * @brief Yield the CPU to the next Ready task.
 *
 * Rust equivalent: `r#yield`.
 */
long user_yield();

/**
 * @brief Get current time in milliseconds.
 *
 * Rust equivalent: `get_time`.
 */
long user_get_time();

/**
 * @brief Change the data segment size.
 *
 * Rust equivalent: `sbrk`.
 *
 * @param size Signed delta for the program break.
 * @return The old program break on success, or -1 on failure.
 */
long user_sbrk(int size);

#endif /* __USER_LIB_H_ */
