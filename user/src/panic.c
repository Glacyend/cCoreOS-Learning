/**
 * @file panic.c
 * @brief User-space panic handler.
 */

#include "panic.h"
#include "console.h"

#include <stdarg.h>
#include <stddef.h>

[[noreturn]] void user_panic_raw(const char* file, int line, const char* fmt, ...) {
    if (file != NULL) {
        user_printf("Panicked at %s:%d : ", file, line);
    } else {
        user_printf("Panicked: ");
    }
    va_list ap;
    va_start(ap, fmt);
    user_vprintf(fmt, ap);
    va_end(ap);
    user_puts("\n");
    // rust equivalent: `loop {}`
    while (true) {}
}
