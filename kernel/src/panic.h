/**
 * @file panic.h
 * @brief Kernel panic handler.
 *
 * Mirrors the Rust `#[panic_handler]`: prints the panic location and message,
 * then shuts the machine down with `SystemFailure`.
 */

#ifndef __PANIC_H_
#define __PANIC_H_

/**
 * @brief Print a panic message and shut down.
 *
 * @param file Source file where the panic occurred.
 * @param line Source file where the panic occurred.
 * @param fmt  Format string for the panic message.
 * @param ...  Variadic arguments.
 */
[[noreturn]] void panic_raw(const char* file, int line, const char* fmt, ...);

/**
 * @brief Panic with the caller's file and line.
 *
 * Usage: `panic("bad value: %d", value);`
 */
#define panic(fmt, ...) panic_raw(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Assert that @p cond is true; panic otherwise.
 *
 * Usage: `assert(ptr != NULL);`
 */
#define assert_panic(cond)                              \
    do {                                          \
        if (!(cond)) {                            \
            panic("assertion failed: %s", #cond); \
        }                                         \
    } while (false);

#endif /* __PANIC_H_ */
