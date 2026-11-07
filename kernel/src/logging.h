/**
 * @file logging.h
 * @brief Leveled logging for the kernel.
 *
 * Mirrors the Rust `logging.rs`: each message is prefixed with an
 * ANSI-colored, centered level tag and followed by a newline.
 *
 * The maximum level is a compile-time option `LOG_LEVEL` (0..4). If not
 * defined, defaults to `2` (INFO).
 */

#ifndef __LOGGING_H_
#define __LOGGING_H_

// Level constants, ordered from most to least severe.
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TRACE 4

/** @brief Log level type. */
typedef int LogLevel;

/**
 * @brief Initialize the logging subsystem.
 *
 * Sets the maximum level from the compile-time @c LOG_LEVEL_DEFAULT macro
 * (defaults to @ref LOG_LEVEL_INFO ).
 */
void log_init();

/**
 * @brief Override the maximum enabled log level at runtime.
 *
 * @param level New maximum level.
 */
void log_set_max_level(LogLevel level);

/**
 * @brief Return the current maximum enabled log level.
 *
 * @return The current maximum log level.
 */
LogLevel log_get_max_level();

/**
 * @brief Emit one log record.
 *
 * If @p level is above the current maximum, the function returns immediately
 * without consuming the arguments.
 *
 * @param level Severity of the record.
 * @param fmt   Format string (see @ref console_printf ).
 * @param ...   Variadic arguments.
 */
void log_write(LogLevel level, const char* fmt, ...);

/** @brief Log at @ref LOG_LEVEL_ERROR . */
#define log_error(fmt, ...) log_write(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

/** @brief Log at @ref LOG_LEVEL_WARN . */
#define log_warn(fmt, ...) log_write(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)

/** @brief Log at @ref LOG_LEVEL_INFO . */
#define log_info(fmt, ...) log_write(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

/** @brief Log at @ref LOG_LEVEL_DEBUG . */
#define log_debug(fmt, ...) log_write(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

/** @brief Log at @ref LOG_LEVEL_TRACE . */
#define log_trace(fmt, ...) log_write(LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

// Short aliases matching the Rust `error!`/`warn!`/... macros.
#define error(fmt, ...) log_error(fmt, ##__VA_ARGS__)
#define warn(fmt, ...) log_warn(fmt, ##__VA_ARGS__)
#define info(fmt, ...) log_info(fmt, ##__VA_ARGS__)
#define debug(fmt, ...) log_debug(fmt, ##__VA_ARGS__)
#define trace(fmt, ...) log_trace(fmt, ##__VA_ARGS__)

#endif
