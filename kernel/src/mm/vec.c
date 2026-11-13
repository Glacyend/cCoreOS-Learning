/**
 * @file vec.c
 * @brief A dynamically-sized vector, the C counterpart of `alloc::vec::Vec`.
 */

#include "vec.h"
#include "../panic.h"
#include "heap_allocator.h"

#include <stddef.h>

/**
 * @brief Initial capacity when the first element is pushed.
 */
#define VEC_INITIAL_CAP 4

static void bytes_copy(void* dst, const void* src, usize len) {
    uint8_t* dst_bytes = (uint8_t*)dst;
    const uint8_t* src_bytes = (const uint8_t*)src;
    for (usize i = 0; i < len; i++) {
        dst_bytes[i] = src_bytes[i];
    }
}

void vec_init(Vector* vec, usize elem_size) {
    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
    vec->elem_size = elem_size;
}

void vec_free(Vector* vec) {
    if (vec->data != NULL) {
        kfree(vec->data);
    }
    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
}

/**
 * @brief Grow the backing buffer to at least `len + 1` elements.
 *
 * Doubles the capacity, or starts at @ref VEC_INITIAL_CAP .
 *
 * Rust equivalent: `Vec::reserve`.
 */
static bool vec_grow(Vector* vec) {
    usize new_cap = (vec->cap == 0) ? VEC_INITIAL_CAP : vec->cap * 2;
    void* new_data = kmalloc(new_cap * vec->elem_size);
    if (new_data == NULL) {
        return false;
    }
    if (vec->data != NULL) {
        bytes_copy(new_data, vec->data, vec->len * vec->elem_size);
        kfree(vec->data);
    }
    vec->data = new_data;
    vec->cap = new_cap;
    return true;
}

bool vec_push(Vector* vec, const void* elem) {
    if (vec->len == vec->cap) {
        if (!vec_grow(vec)) {
            return false;
        }
    }
    uint8_t* dst = (uint8_t*)vec->data + vec->len * vec->elem_size;
    bytes_copy(dst, elem, vec->elem_size);
    vec->len++;
    return true;
}

bool vec_pop(Vector* vec, void* out) {
    if (vec->len == 0) {
        return false;
    }
    vec->len--;
    if (out != NULL) {
        const uint8_t* src = (const uint8_t*)vec->data + vec->len * vec->elem_size;
        bytes_copy(out, src, vec->elem_size);
    }
    return true;
}

void* vec_get_mut(Vector* vec, usize idx) {
    if (idx >= vec->len) {
        panic("vec_get_mut: index %zu out of range (len = %zu)", idx, vec->len);
    }
    return (uint8_t*)vec->data + idx * vec->elem_size;
}

const void* vec_get(const Vector* vec, usize idx) {
    if (idx >= vec->len) {
        panic("vec_get: index %zu out of range (len = %zu)", idx, vec->len);
    }
    return (const uint8_t*)vec->data + idx * vec->elem_size;
}

usize vec_len(const Vector* vec) {
    return vec->len;
}
