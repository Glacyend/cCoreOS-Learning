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
<<<<<<< Updated upstream
static const usize FD_STDOUT = 1;
=======
static const size_t FD_STDOUT = 1;
>>>>>>> Stashed changes

/**
 * @brief Validate that `[data, data + len)` is well-formed UTF-8.
 *
 * Rust equivalent: `core::str::from_utf8(...).is_ok()`.
 *
 * Rejects overlong encodings, surrogate code points, and code points above
 * `U+10FFFF`, matching the checks performed by Rust's `char::from_u32`.
 */
<<<<<<< Updated upstream
static bool utf8_validate(const uint8_t* data, usize len) {
    usize i = 0;
    while (i < len) {
        uint8_t first = data[i];
        usize seq_len;
=======
static bool utf8_validate(const char* data, size_t len) {
    size_t i = 0;
    while (i < len) {
        uint8_t first = data[i];
        size_t seq_len;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        for (usize k = 1; k < seq_len; k++) {
=======
        for (size_t k = 1; k < seq_len; k++) {
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
=======
long sys_write(size_t fd, const char* buf, size_t len) {
    if (fd == FD_STDOUT) {
        Vector buffers;
        translated_byte_buffer(&buffers, task_current_user_token(), buf, len);
        size_t buffer_count = vec_len(&buffers);
        for (size_t i = 0; i < buffer_count; i++) {
            const ByteSlice* slice = (const ByteSlice*)vec_get(&buffers, i);
            if (!utf8_validate((const char*)slice->data, slice->len)) {
                panic("sys_write: invalid UTF-8");
            }
            for (size_t j = 0; j < slice->len; j++) {
>>>>>>> Stashed changes
                console_putchar((char)slice->data[j]);
            }
        }
        vec_free(&buffers);
<<<<<<< Updated upstream
        return (isize)len;
=======
        return (long)len;
>>>>>>> Stashed changes
    } else {
        panic("Unsupported fd in sys_write!");
    }
}
