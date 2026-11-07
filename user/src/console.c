/**
 * @file console.c
 * @brief User-side formatted output.
 *
 * Every character is emitted with a one-byte `sys_write`; the programs are
 * small enough that this is fine.
 */

#include "console.h"
#include "syscall.h"

#include <stdarg.h>
#include <stddef.h>

void user_puts(const char* s) {
    usize len = 0;
    while (s[len] != '\0') {
        len++;
    }
    sys_write(1, s, len);
}

static void putchar(char c) {
    sys_write(1, &c, 1);
}

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

static void print_signed(long value) {
    if (value < 0) {
        putchar('-');
        unsigned long uv = (unsigned long)(-(value + 1)) + 1ul;
        print_unsigned(uv, 10, false);
    } else {
        print_unsigned((unsigned long)value, 10, false);
    }
}

void user_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    while (*fmt != '\0') {
        if (*fmt != '%') {
            putchar(*fmt++);
            continue;
        }
        fmt++;
        switch (*fmt) {
            case 'c': {
                putchar((char)va_arg(ap, int));
                break;
            }
            case 's': {
                const char* str = va_arg(ap, const char*);
                user_puts(str != NULL ? str : "(null)");
                break;
            }
            case 'd':
            case 'i': {
                print_signed(va_arg(ap, int));
                break;
            }
            case 'u': {
                print_unsigned(va_arg(ap, unsigned int), 10, false);
                break;
            }
            case 'x': {
                print_unsigned(va_arg(ap, unsigned int), 16, false);
                break;
            }
            case 'X': {
                print_unsigned(va_arg(ap, unsigned int), 16, true);
                break;
            }
            case 'l': {
                fmt++;
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
                    default: {
                        user_puts("%l?");
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

    va_end(ap);
}
