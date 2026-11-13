/**
 * @file heap_allocator.h
 * @brief The global allocator.
 */

#ifndef __MM_HEAP_ALLOCATOR_H_
#define __MM_HEAP_ALLOCATOR_H_

#include "../config.h"

/**
 * @brief Initiate heap allocator.
 *
 * Must be called once before any call of @ref kmalloc or @ref kfree.
 */
void init_heap();

/**
 * @brief Allocate @p size bytes from the kernel heap.
 *
 * Rust equivalent: `alloc::alloc::alloc`, called implicitly by `Vec` and
 * friends.
 *
 * @param size Requested size in bytes.
 * @return Pointer to the allocated block, or `NULL` in out-of-memory.
 */
void* kmalloc(usize size);

/**
 * @brief Free a block previously returned by @ref kmalloc .
 *
 * Rust equivalent: `alloc::alloc::dealloc`. Passing `NULL` is a no-op.
 */
void kfree(void* ptr);

#endif /* __MM_HEAP_ALLOCATOR_H_ */
