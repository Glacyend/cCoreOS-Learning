/**
 * @file console.h
 * @brief Text output over the SBI console.
 *
 * Provides a minimal `printf` implementation for the kernel. Only a subset of
 * format specifiers is supported; see `console_printf` for the full list.
 */

#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <stdarg.h>

/**
 * @brief Write one character to the console.
 *
 * @param c Character to print.
 */
void console_putchar(char c);

/**
 * @brief Write a NUL-terminated string to the console.
 *
 * @param s String to print.
 */
void console_write(const char* s);

/**
 * @brief `va_list` form of @ref console_printf .
 *
 * Used by wrappers such as the logging system, which forward their own
 * `va_list`.
 *
 * @param fmt Format string.
 * @param ap  Argument list.
 */
void console_vprintf(const char* fmt, va_list ap);

/**
 * @brief Formatted output to the console.
 *
 * @param fmt Format string.
 * @param ... Variadic arguments.
 */
void console_printf(const char* fmt, ...);

/**
 * @brief Kernel `print!` equivalent.
 *
 * Expands to a `console_printf` call. With no arguments, prints `fmt`
 * verbatim.
 */
#define print(fmt, ...) console_printf(fmt, ##__VA_ARGS__)

/**
 * @brief Kernel `println!` equivalent.
 *
 * Same as @ref print but appends a newline.
 */
#define println(fmt, ...)                   \
    do {                                    \
        console_printf(fmt, ##__VA_ARGS__); \
        console_putchar('\n');              \
    } while (false);

#endif /* __CONSOLE_H_ */
