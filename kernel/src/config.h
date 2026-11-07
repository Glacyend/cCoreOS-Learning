/**
 * @file config.h
 * @brief Constants used in cCoreOS.
 *
 * All magic numbers that describe the kernel's memory layout and scheduling
 * policy live here. Other modules should include this header instead of
 * hand-coding addresses or sizes.
 */

#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <stdint.h>

/** @brief Pointer-width unsigned integer. */
typedef uint64_t usize;

/** @brief Pointer-width signed integer. */
typedef int64_t isize;

/** @brief Size of each user task's user stack (8 KiB). */
#define USER_STACK_SIZE (4096 * 2)

/** @brief Size of each user task's kernel stack (8 KiB). */
#define KERNEL_STACK_SIZE (4096 * 2)

/** @brief Size of the kernel heap (3 MiB). */
#define KERNEL_HEAP_SIZE (0x00300000)

/** @brief Page size (4 KiB). */
#define PAGE_SIZE 0x1000

/** @brief log2(PAGE_SIZE), used for address/page-number conversion. */
#define PAGE_SIZE_BITS 0xC

/**
 * @brief Virtual address of the trampoline page.
 *
 * Mapped to the same physical page in both the kernel and every user page
 * table, so that the PC remains valid across `satp` switches.
 */
#define TRAMPOLINE (UINT64_MAX - PAGE_SIZE + 1)

/** @brief Virtual address of the current task's `TrapContext`. */
#define TRAP_CONTEXT (TRAMPOLINE - PAGE_SIZE)

/** @brief End of usable physical memory on QEMU virt (128 MiB). */
#define MEMORY_END 0x88000000ul

/**
 * @brief Compute the virtual address range of task @p app_id 's kernel stack.
 *
 * Kernel stacks are laid out downwards from @ref TRAMPOLINE , with a guard page
 * between adjacent stacks to catch overflow.
 *
 * @param[in] app_id  Task id.
 * @param[out] bottom Lowest address of the stack.
 * @param[out] top    Highest address of the stack (initial `sp`).\
 */
static inline void kernel_stack_position(usize app_id, usize* bottom, usize* top) {
    *top = TRAMPOLINE - app_id * (KERNEL_STACK_SIZE + PAGE_SIZE);
    *bottom = *top - KERNEL_STACK_SIZE;
}

/** @brief Timer interrupts per second (100 Hz -> 10 ms time slice). */
#define TICKS_PER_SEC 100

/** @brief Milliseconds per second, used by `get_time_ms`. */
#define MSEC_PER_SEC 1000

/** @brief Maximum number of applications statically embedded. */
#define MAX_APP_NUM 4

/** @brief Base address where every app is loaded. */
#define APP_BASE_ADDRESS 0x80400000ul

/** @brief Fixed slot size per app; apps must fit within it. */
#define APP_SIZE_LIMIT 0x20000ul

#endif /* __CONFIG_H_ */
