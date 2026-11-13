/**
 * @file qemu.h
 * @brief Constants used in cCore for qemu.
 */

#ifndef __BOARDS_QEMU_H_
#define __BOARDS_QEMU_H_

#include "../config.h"

/**
 * @brief End of usable physical memory.
 */
#define MEMORY_END 0x88000000ul

/**
 * @brief A physical address range: `(start, length)`.
 *
 * Rust equivalent: `(usize, usize)`.
 */
typedef struct {
    usize start;
    usize len;
} MMIORange;

/**
 * @brief MMIO regions that must be kept out of the frame allocator.
 *
 * Rust equivalent: `&[(usize, usize)]`. The slice length is exposed separately
 * as @ref MMIO_LEN .
 */
extern const MMIORange MMIO[];

/**
 * @brief Number of entries in @ref MMIO .
 */
extern const usize MMIO_LEN;

#endif /* __BOARDS_QEMU_H_ */
