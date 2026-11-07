/**
 * @file timer.c
 * @brief RISC-V timer-related functionality.
 */

#include "timer.h"
#include "device_tree.h"
#include "sbi.h"

/**
 * @brief Read `mtime` via the `rdtime` instruction.
 *
 * `rdtime` is available in S-mode in all RISC-V platforms that expose the
 * `time` CSR.
 *
 * @return The current `mtime` value.
 */
static inline usize read_mtime() {
    usize t;
    __asm__ volatile("rdtime %0" : "=r"(t));
    return t;
}

usize timer_get_time() {
    return read_mtime();
}

usize timer_get_time_ms() {
    return read_mtime() / (device_tree_clock_freq() / MSEC_PER_SEC);
}

void timer_set_next_trigger() {
    usize next = read_mtime() + device_tree_clock_freq() / TICKS_PER_SEC;
    sbi_set_timer(next);
}
