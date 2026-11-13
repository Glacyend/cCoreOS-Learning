/**
 * @file fs.c
 * @brief File and filesystem-related syscalls.
 *
 * Rust uses `print!("{}", str)` which goes through `chars()` and sends each
 * Unicode code point to `sbi::console_putchar`. SBI's legacy `console_putchar`
 * only keeps the low byte, so non-ASCII characters would be truncated in Rust.
 * The C version emits raw bytes instead, which keeps UTF-8 output correct and
 * does not change ASCII.
 */

#include "fs.h"
#include "../console.h"
#include "../mm/page_table.h"
#include "../mm/slice.h"
#include "../panic.h"
#include "../task/task.h"

/**
 * @brief File descriptor for standard output.
 *
 * Rust equivalent: `FD_STDOUT`.
 */
static const usize FD_STDOUT = 1;

/**
 * @brief Validate that `[data, data + len)` is well-formed UTF-8.
 *
 * Rust equivalent: `core::str::from_utf8(...).is_ok()`.
 *
 * Rejects overlong encodings, surrogate code points, and code points above
 * `U+10FFFF`, matching the checks performed by Rust's `char::from_u32`.
 */
static bool utf8_validate(const uint8_t* data, usize len) {
    usize i = 0;
    while (i < len) {
        uint8_t first = data[i];
        usize seq_len;
        uint32_t codepoint;
        if ((first & 0x80) == 0) {
            seq_len = 1;
            codepoint = (uint32_t)first;
        } else if ((first & 0xE0) == 0xC0) {
            seq_len = 2;
            codepoint = (uint32_t)(first & 0x1F);
        } else if ((first & 0xF0) == 0xE0) {
            seq_len = 3;
            codepoint = (uint32_t)(first & 0x0F);
        } else if ((first & 0xF8) == 0xF0) {
            seq_len = 4;
            codepoint = (uint32_t)(first & 0x07);
        } else {
            return false;
        }
        if (i + seq_len > len) {
            return false;
        }
        for (usize k = 1; k < seq_len; k++) {
            uint8_t cont = data[i + k];
            if ((cont & 0xC0) != 0x80) {
                return false;
            }
            codepoint = (codepoint << 6) | (uint32_t)(cont & 0x3F);
        }
        if (seq_len == 2 && codepoint < 0x80) {
            return false;
        }
        if (seq_len == 3 && codepoint < 0x800) {
            return false;
        }
        if (seq_len == 4 && codepoint < 0x10000) {
            return false;
        }
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
            return false;
        }
        if (codepoint >= 0x10FFFF) {
            return false;
        }
        i += seq_len;
    }
    return true;
}

isize sys_write(usize fd, const uint8_t* buf, usize len) {
    if (fd == FD_STDOUT) {
        Vector buffers;
        translated_byte_buffer(&buffers, task_current_user_token(), buf, len);
        usize buffer_count = vec_len(&buffers);
        for (usize i = 0; i < buffer_count; i++) {
            const ByteSlice* slice = (const ByteSlice*)vec_get(&buffers, i);
            if (!utf8_validate(slice->data, slice->len)) {
                panic("sys_write: invalid UTF-8");
            }
            for (usize j = 0; j < slice->len; j++) {
                console_putchar((char)slice->data[j]);
            }
        }
        vec_free(&buffers);
        return (isize)len;
    } else {
        panic("Unsupported fd in sys_write!");
    }
}
