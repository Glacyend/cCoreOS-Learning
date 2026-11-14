/**
 * @file timer.c
 * @brief RISC-V timer-related functionality.
 */

#include "timer.h"
#include "device_tree.h"
#include "sbi.h"

/**
 * @brief Timer interrupts per second.
 *
 * Rust equivalent: `TICKS_PER_SEC`.
 */
static const size_t TICKS_PER_SEC = 100;

/**
 * @brief Milliseconds per second.
 *
 * Rust equivalent: `MSEC_PER_SEC`.
 */
static const size_t MSEC_PER_SEC = 1000;

/**
 * @brief Read the `time` CSR via the `rdtime` instruction.
 *
 * Rust equivalent: `riscv::register::time::read`.
 */
static inline size_t read_mtime() {
    size_t t;
    __asm__ volatile("rdtime %0" : "=r"(t));
    return t;
}

size_t timer_get_time() {
    return read_mtime();
}

size_t timer_get_time_ms() {
    return read_mtime() / (device_tree_clock_freq() / MSEC_PER_SEC);
}

void timer_set_next_trigger() {
    sbi_set_timer(timer_get_time() + device_tree_clock_freq() / TICKS_PER_SEC);
}
