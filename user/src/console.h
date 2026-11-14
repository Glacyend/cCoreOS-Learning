/**
 * @file console.h
 * @brief User-side formatted output.
 */

#ifndef __USER_CONSOLE_H_
#define __USER_CONSOLE_H_

#include <stdarg.h>

/**
 * @brief Write a NUL-terminated string to stdout.
 *
 * Rust equivalent: `Write::write_str` for `Stdout`, which calls
 * `write(STDOUT, s.as_bytes())` once.
 *
 * @param str String to print.
 */
void user_puts(const char* str);

/**
 * @brief `va_list` form of @ref user_printf .
 *
 * Rust equivalent: the body of `print`, which drives `Stdout.write_fmt(args)`.
 *
 * @param fmt Format string.
 * @param ap  Argument list.
 */
void user_vprintf(const char* fmt, va_list ap);

/**
 * @brief Formatted output to stdout.
 *
 * Rust equivalent: `print!`. Supported format specifiers match the kernel
 * `console_printf`.
 *
 * @param fmt Format string.
 * @param ... Variadic arguments.
 */
void user_printf(const char* fmt, ...);

#endif /* __USER_CONSOLE_H_ */
