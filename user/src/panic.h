/**
 * @file panic.h
 * @brief User-space panic handler.
 *
 * Rust equivalent: `lang_items:panic_handler`. C has no `#[panic_handler]`
 * language item; panics are raised explicitly through the
 * `user_panic(fmt, ...)` macro.
 */

#ifndef __USER_PANIC_H_
#define __USER_PANIC_H_

/**
 * @brief Print a panic message and loop forever.
 *
 * Rust equivalent: `panic_handler`. If @p file is non-`NULL`, prints
 * `Panicked at <file>:<line>, <msg>`; otherwise prints `Panicked: <msg>`. Then
 * enters an infinite loop, matching the Rust `loop {}`.
 *
 * @param file Source file where the panic occurred, or `NULL`.
 * @param line Source line where the panic occurred.
 * @param fmt  Format string for the panic message.
 * @param ...  Variadic arguments.
 */
[[noreturn]] void user_panic_raw(const char* file, int line, const char* fmt, ...);

/**
 * @brief Panic with the caller's file and line.
 *
 * Usage: `user_panic("bad value: %d", value);`
 */
#define user_panic(fmt, ...) user_panic_raw(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* __USER_PANIC_H_ */
