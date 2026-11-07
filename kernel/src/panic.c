/**
 * @file panic.c
 * @brief Kernel panic handler.
 */

#include "panic.h"
#include "console.h"
#include "sbi.h"

[[noreturn]] void panic_raw(const char* file, int line, const char* fmt, ...) {
    print("\x1b[31m[ ERROR  ] Panicked at %s:%d: ", file, line);

    va_list ap;
    va_start(ap, fmt);
    console_vprintf(fmt, ap);
    va_end(ap);

    println("\x1b[0m");

    sbi_shutdown(true);
}
