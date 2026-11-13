/**
 * @file vec.h
 * @brief A dynamically-sized vector, the C counterpart of `alloc::vec::Vec`.
 *
 * `Vec<T>` is generic in Rust; C has no generics, so `Vector` stores an
 * untyped byte buffer plus the element size. `vec_push` and `vec_pop` copy
 * `elem_size` bytes per element. The caller is responsible for any per-element
 * cleanup before calling @ref vec_free , because C has no `Drop`.
 */

#ifndef __MM_VEC_H_
#define __MM_VEC_H_

#include "../config.h"

/**
 * @brief A dynamically-sized vector.
 *
 * Rust equivalent: `alloc::vec::Vec<T>`. `elem_size` is fixed at @ref vec_init
 * time.
 */
typedef struct {
    /** @brief Backing buffer, or `NULL` if empty. */
    void* data;

    /** @brief Number of live elements. */
    usize len;

    /** @brief Number of elements the buffer can hold. */
    usize cap;

    /** @brief Size of one element in bytes. */
    usize elem_size;
} Vector;

/**
 * @brief Initialize an empty vector for elements of @p elem_size bytes.
 *
 * Rust equivalent: `Vec::<T>::new()`.
 *
 * @param vec       Vector to initialize.
 * @param elem_size Size of one element in bytes.
 */
void vec_init(Vector* vec, usize elem_size);

/**
 * @brief Free the backing buffer.
 *
 * Does not free per-element resources; callers must do that themselves.
 *
 * Rust equivalent: dropping a `Vec<T>`.
 */
void vec_free(Vector* vec);

/**
 * @brief Append a copy of `*elem`.
 *
 * Rust equivalent: `Vec::push`.
 *
 * @param vec  Vector.
 * @param elem Pointer to the element to copy.
 * @return `true` on success, `false` on out-of-memory.
 */
bool vec_push(Vector* vec, const void* elem);

/**
 * @brief Remove the last element and write it to `*out`.
 *
 * Rust equivalent: `Vec::pop`.
 *
 * @param      vec Vector.
 * @param[out] out Receives the popped element, or `NULL` to discard it.
 * @return `true` if an element was popped, `false` if the vector is empty.
 */
bool vec_pop(Vector* vec, void* out);

/**
 * @brief Returns a mutable pointer to element @p idx .
 *
 * Rust equivalent: `&mut Vec<T>[idx]`. The caller must cast to the actual
 * element type.
 *
 * @param vec Vector.
 * @param idx Index, `0 <= idx < vec->len`.
 * @return Pointer to the element.
 */
void* vec_get_mut(Vector* vec, usize idx);

/**
 * @brief Return a const pointer to element @p idx .
 *
 * Rust equivalent: `&Vec<T>[idx]`.
 */
const void* vec_get(const Vector* vec, usize idx);

/**
 * @brief Number of live elements.
 *
 * Rust equivalent: `Vec::len`.
 */
usize vec_len(const Vector* vec);

#endif /* __MM_VEC_H_ */
