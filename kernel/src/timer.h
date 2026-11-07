/**
 * @file timer.h
 * @brief RISC-V timer-related functionality.
 *
 * `mtime` is a free-running 64-bit counter whose frequency is reported by the
 * device tree as `timebase-frequency`. The kernel programs the next timer
 * inerrupt via SBI `set_timer`, which writes `mtimecmp` in M-mode.
 */

#ifndef __TIMER_H_
#define __TIMER_H_

#include "config.h"

/**
 * @brief Read the `mtime` register.
 *
 * @return Current `mtime` value, in ticks.
 */
usize timer_get_time();

/**
 * @brief Current time in milliseconds.
 *
 * @return Milliseconds since boot.
 */
usize timer_get_time_ms();

/**
 * @brief Program the next timer interrupt.
 *
 * Sets `mtimecmp = now + clock_freq / TICKS_PER_SEC`, i.e. the next interrupt
 * fires after one time slice (10ms by default).
 */
void timer_set_next_trigger();

#endif /* __TIMER_H_ */
