/**
 * @file panic.c
 * @brief The panic handler.
 */

#include "panic.h"
#include "console.h"
#include "sbi.h"

#include <stddef.h>

[[noreturn]] void panic_raw(const char* file, int line, const char* fmt, ...) {
    if (file != NULL) {
        print("\x1b[31m[ ERROR  ] [kernel] Panicked at %s:%d : ", file, line);
        va_list ap;
        va_start(ap, fmt);
        console_vprintf(fmt, ap);
        va_end(ap);
        println("\x1b[0m");
    } else {
        print("\x1b[31m[ ERROR  ] [kernel] Panicked: ");
    }

    sbi_shutdown(true);
}
