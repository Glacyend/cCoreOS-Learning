/**
 * @file device_tree.h
 * @brief Minimal flattened device tree (FDT) helpers.
 *
 * Only the pieces needed by the kernel are implemented: locating the
 * `timebase-frequency` property under `/cpus`. The full FDT spec is at
 * Devicetree Specification v0.4.
 *
 * The DTB physical address is passed by OpenSBI in `a1`, which `entry.S`
 * forwards to `kernel_main` unchanged.
 */

#ifndef __DEVICE_TREE_H_
#define __DEVICE_TREE_H_

#include "config.h"

/**
 * @brief Parse the DTB and record the CPU timebase frequency.
 *
 * Scans the FDT structure block for a property named `timebase-frequency` and
 * stores its value in a global.
 *
 * @param dtb_pa Physical address of the device tree blob.
 * @return 0 on success, -1 if the DTB is malformed or the property is missing.
 */
int device_tree_init(usize dtb_pa);

/**
 * @brief Return the `mtime` ticks per second.
 *
 * @return The `timebase-frequency` read by @ref device_tree_init .
 * @pre @ref device_tree_init must have succeeded.
 */
usize device_tree_clock_freq();

#endif /* __DEVICE_TREE_H_ */
