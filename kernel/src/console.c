/**
 * @file console.c
 * @brief Text output over the SBI console.
 */

#include "console.h"
#include "sbi.h"

#include <stdarg.h>
#include <stddef.h>

void console_putchar(char c) {
    sbi_console_putchar((usize)(unsigned char)c);
}

void console_write(const char* s) {
    while (*s != '\0') {
        console_putchar(*s++);
    }
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
        console_putchar('0');
        return;
    }

    while (value > 0) {
        buf[i++] = digits[value % (unsigned)base];
        value /= (unsigned)base;
    }

    while (i > 0) {
        console_putchar(buf[--i]);
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
        console_putchar('-');
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
    console_write("0x");
    for (int shift = 60; shift >= 0; shift -= 4) {
        int d = (int)((value >> shift) & 0xFul);
        console_putchar((d < 10) ? (char)('0' + d) : (char)('A' + d - 10));
    }
}

void console_vprintf(const char* fmt, va_list ap) {
    while (*fmt != '\0') {
        if (*fmt != '%') {
            console_putchar(*fmt++);
            continue;
        }
        fmt++;  // skip `%`
        if (*fmt == '\0') {
            console_putchar('%');
            break;
        }

        switch (*fmt) {
            case 'c': {
                int c = va_arg(ap, int);
                console_putchar((char)c);
                break;
            }
            case 's': {
                const char* s = va_arg(ap, const char*);
                console_write(s != NULL ? s : "(null)");
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
                                console_write("%ll?");
                                break;
                            }
                        }
                        break;
                    }
                    default: {
                        console_write("%l?");
                        break;
                    }
                }
                break;
            }
            case 'z': {
                fmt++;
                switch (*fmt) {
                    case 'u': {
                        print_unsigned((unsigned long)va_arg(ap, usize), 10, false);
                        break;
                    }
                    case 'd':
                    case 'i': {
                        print_signed((long)va_arg(ap, isize));
                        break;
                    }
                    case 'x': {
                        print_unsigned((unsigned long)va_arg(ap, usize), 16, false);
                        break;
                    }
                    case 'X': {
                        print_unsigned((unsigned long)va_arg(ap, usize), 16, true);
                        break;
                    }
                    default: {
                        console_write("%z?");
                        break;
                    }
                }
                break;
            }
            case '%': {
                console_putchar('%');
                break;
            }
            default: {
                console_write("%?");
                break;
            }
        }
        fmt++;
    }
}

void console_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    console_vprintf(fmt, ap);
    va_end(ap);
}
