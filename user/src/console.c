/**
 * @file console.c
 * @brief User-side formatted output.
 *
 * Each `putchar` issues a one-byte `sys_write`; `user_puts` issues a single
 * `sys_write` for a whole string, matching the Rust `Write for Stdout`
 * implementation in `console.rs`, which calls `write(STDOUT, s.as_bytes())`
 * once per `write_str`.
 */

#include "console.h"
#include "syscall.h"

#include <stdarg.h>
#include <stddef.h>

/**
 * @brief Write one character to stdout.
 *
 * Rust equivalent: a single iteration of `s.as_bytes()` inside
 * `Write::write_str`.
 */
static void putchar(char ch) {
    sys_write(1, &ch, 1);
}

void user_puts(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    sys_write(1, str, len);
}

/**
 * @brief Print an unsigned integer in the given base.
 *
 * @param value     Value to print.
 * @param base      10 or 16.
 * @param uppercase `true` to use `A`..`F` in hex.
 */
static void print_unsigned(unsigned long value, int base, bool uppercase) {
    static const char lower[] = "0123456789abcdef";
    static const char upper[] = "0123456789ABCDEF";
    const char* digits = uppercase ? upper : lower;

    char buf[24];
    int i = 0;

    if (value == 0) {
        putchar('0');
        return;
    }

    while (value > 0) {
        buf[i++] = digits[value % (unsigned)base];
        value /= (unsigned)base;
    }

    while (i > 0) {
        putchar(buf[--i]);
    }
}

/**
 * @brief Print a signed integer in decimal.
 *
 * Handles `LONG_MIN` correctly by avoiding `-v` overflow.
 *
 * @param value Value to print.
 */
static void print_signed(long value) {
    if (value < 0) {
        putchar('-');
        unsigned long uv = (unsigned long)(-(value + 1)) + 1ul;
        print_unsigned(uv, 10, false);
    } else {
        print_unsigned((unsigned long)value, 10, false);
    }
}

/**
 * @brief Print a pointer as `0x` followed by 16 hex digits.
 *
 * @param value Pointer to print.
 */
static void print_pointer(unsigned long value) {
    user_puts("0x");
    for (int shift = 60; shift >= 0; shift -= 4) {
        int d = (int)((value >> shift) & 0xFul);
        putchar((d < 10) ? (char)('0' + d) : (char)('A' + d - 10));
    }
}

void user_vprintf(const char* fmt, va_list ap) {
    while (*fmt != '\0') {
        if (*fmt != '%') {
            putchar(*fmt++);
            continue;
        }
        fmt++;  // skip `%`
        if (*fmt == '\0') {
            putchar('%');
            break;
        }

        switch (*fmt) {
            case 'c': {
                int c = va_arg(ap, int);
                putchar((char)c);
                break;
            }
            case 's': {
                const char* s = va_arg(ap, const char*);
                user_puts(s != NULL ? s : "(null)");
                break;
            }
            case 'd':
            case 'i': {
                print_signed((long)va_arg(ap, int));
                break;
            }
            case 'u': {
                print_unsigned((unsigned long)va_arg(ap, unsigned int), 10, false);
                break;
            }
            case 'x': {
                print_unsigned((unsigned long)va_arg(ap, unsigned int), 16, false);
                break;
            }
            case 'X': {
                print_unsigned((unsigned long)va_arg(ap, unsigned int), 16, true);
                break;
            }
            case 'p': {
                print_pointer((unsigned long)va_arg(ap, void*));
                break;
            }
            case 'l': {
                fmt++;
                if (*fmt == '\0') {
                    user_puts("%l");
                    return;
                }
                switch (*fmt) {
                    case 'd':
                    case 'i': {
                        print_signed(va_arg(ap, long));
                        break;
                    }
                    case 'u': {
                        print_unsigned(va_arg(ap, unsigned long), 10, false);
                        break;
                    }
                    case 'x': {
                        print_unsigned(va_arg(ap, unsigned long), 16, false);
                        break;
                    }
                    case 'X': {
                        print_unsigned(va_arg(ap, unsigned long), 16, true);
                        break;
                    }
                    case 'l': {
                        fmt++;
                        if (*fmt == '\0') {
                            user_puts("%ll");
                            return;
                        }
                        switch (*fmt) {
                            case 'd':
                            case 'i': {
                                print_signed((long)va_arg(ap, long long));
                                break;
                            }
                            case 'u': {
                                print_unsigned(
                                    (unsigned long)va_arg(ap, unsigned long long), 10, false
                                );
                                break;
                            }
                            case 'x': {
                                print_unsigned(
                                    (unsigned long)va_arg(ap, unsigned long long), 16, false
                                );
                                break;
                            }
                            case 'X': {
                                print_unsigned(
                                    (unsigned long)va_arg(ap, unsigned long long), 16, true
                                );
                                break;
                            }
                            default: {
                                user_puts("%ll?");
                                break;
                            }
                        }
                        break;
                    }
                    default: {
                        user_puts("%l?");
                        break;
                    }
                }
                break;
            }
            case 'z': {
                fmt++;
                if (*fmt == '\0') {
                    user_puts("%z");
                    return;
                }
                switch (*fmt) {
                    case 'u': {
                        print_unsigned((unsigned long)va_arg(ap, size_t), 10, false);
                        break;
                    }
                    case 'd':
                    case 'i': {
                        print_signed((long)va_arg(ap, long));
                        break;
                    }
                    case 'x': {
                        print_unsigned((unsigned long)va_arg(ap, size_t), 16, false);
                        break;
                    }
                    case 'X': {
                        print_unsigned((unsigned long)va_arg(ap, size_t), 16, true);
                        break;
                    }
                    default: {
                        user_puts("%z?");
                        break;
                    }
                }
                break;
            }
            case '%': {
                putchar('%');
                break;
            }
            default: {
                user_puts("%?");
                break;
            }
        }
        fmt++;
    }
}

void user_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    user_vprintf(fmt, ap);
    va_end(ap);
}
