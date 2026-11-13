/**
 * @file config.h
 * @brief Constants used in cCoreOS.
 */

#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <stdint.h>

/** @brief Pointer-width unsigned integer. */
typedef uint64_t usize;

/** @brief Pointer-width signed integer. */
typedef int64_t isize;

/** @brief Size of each user stack (8 KiB). */
static const usize USER_STACK_SIZE = 4096 * 2;

/** @brief Size of each kernel stack (8 KiB). */
static const usize KERNEL_STACK_SIZE = 4096 * 2;

/** @brief Size of the kernel heap (3 MiB). */
static const usize KERNEL_HEAP_SIZE = 0x00300000;

/** @brief Page size (4 KiB). */
static const usize PAGE_SIZE = 0x1000;

/** @brief log2(PAGE_SIZE), used for address/page-number conversion. */
static const usize PAGE_SIZE_BITS = 0xC;

/** @brief Virtual address of the trampoline page. */
static const usize TRAMPOLINE = UINT64_MAX - PAGE_SIZE + 1;

/** @brief Virtual address of the current task's `TrapContext`. */
static const usize TRAP_CONTEXT = TRAMPOLINE - PAGE_SIZE;

/**
 * @brief Return (bottom, top) of a kernel stack in kernel space.
 *
 * Rust equivalent: `kernel_stack_position`.
 *
 * @param[in]  app_id Task id.
 * @param[out] bottom Lowest address of the stack.
 * @param[out] top    Highest address of the stack.
 */
void kernel_stack_position(usize app_id, usize* bottom, usize* top);

#endif /* __CONFIG_H_ */
