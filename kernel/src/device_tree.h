/**
 * @file device_tree.h
 * @brief Device tree helpers.
 *
 * The DTB physical address is passed by OpenSBI in `a1`, which `entry.S`
 * forwards to `kernel_main` unchanged.
 *
 * Rust uses the `fdt` crate; C uses a hand-written FDT parser that only
 * locates the `timebase-frequency` property under `/cpus`. The full FDT spec
 * is at Devicetree Specification v0.4.
 */

#ifndef __DEVICE_TREE_H_
#define __DEVICE_TREE_H_

#include <stddef.h>

/**
 * @brief Initializes early device information from the flattened device tree.
 *
 * Rust equivalent: `device_tree::init`. Scans the FDT structure block for a
 * property named `timebase-frequency` and stores its value in a global.
 *
 * @param dtb_pa Physical address of the device tree blob.
 * @return 0 on success, -1 if the DTB is malformed or the property is missing.
 */
int device_tree_init(size_t dtb_pa);

/**
 * @brief Returns the number of `mtime` ticks per second.
 *
 * Rust equivalent: `device_tree::clock_freq`.
 *
 * @return The `timebase-frequency` read by @ref device_tree_init .
 * @pre @ref device_tree_init must have succeeded.
 */
size_t device_tree_clock_freq();

#endif /* __DEVICE_TREE_H_ */
