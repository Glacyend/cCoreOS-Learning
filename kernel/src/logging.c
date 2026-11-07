/**
 * @file logging.c
 * @brief Leveled logging for the kernel.
 */

#include "logging.h"
#include "console.h"

#include <stdarg.h>

#ifndef LOG_LEVEL_DEFAULT
#define LOG_LEVEL_DEFAULT LOG_LEVEL_INFO
#endif /* LOG_LEVEL_DEFAULT */

/** @brief Current maximum enabled log level. */
static LogLevel global_max_level = LOG_LEVEL_DEFAULT;

void log_init() {
    global_max_level = LOG_LEVEL_DEFAULT;
}

void log_set_max_level(LogLevel level) {
    global_max_level = level;
}

LogLevel log_get_max_level() {
    return global_max_level;
}

/**
 * @brief Map a level to its ANSI SGR color code.
 *
 * Matches the colors used by Rust `SingleLogger`:
 * Error=31, Warn=93, Info=34, Debug=32, Trace=90.
 *
 * @param level Log level.
 * @return ANSI SGR color code.
 */
static int level_color(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_ERROR: return 31;
        case LOG_LEVEL_WARN: return 93;
        case LOG_LEVEL_INFO: return 34;
        case LOG_LEVEL_DEBUG: return 32;
        case LOG_LEVEL_TRACE: return 90;
        default: return 0;
    }
}

/**
 * @brief Return the printable name of a level.
 *
 * @param level Log level.
 * @return Printable name of the @p level .
 */
static const char* level_name(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_WARN: return "WARN";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_TRACE: return "TRACE";
        default: return "???";
    }
}

/**
 * @brief Print @p level centered within 8 characters.
 *
 * Equivalent to Rust's `format!("{:^8}", level)`.
 *
 * @param level Log level.
 */
static void print_level_centered(LogLevel level) {
    const char* name = level_name(level);

    int len = 0;
    while (name[len] != '\0') {
        len++;
    }

    int total_pad = 8 - len;
    if (total_pad < 0) {
        total_pad = 0;
    }
    int left_pad = total_pad / 2;
    int right_pad = total_pad - left_pad;

    for (int i = 0; i < left_pad; i++) {
        console_putchar(' ');
    }
    console_write(name);
    for (int i = 0; i < right_pad; i++) {
        console_putchar(' ');
    }
}

void log_write(LogLevel level, const char* fmt, ...) {
    if (level > global_max_level) {
        return;
    }

    int color = level_color(level);

    print("\x1b[%dm[", color);
    print_level_centered(level);
    print("] ");

    va_list ap;
    va_start(ap, fmt);
    console_vprintf(fmt, ap);
    va_end(ap);

    println("\x1b[0m");
}
