/**
 * @file timer.h
 * @brief RISC-V timer-related functionality.
 */

#ifndef __TIMER_H_
#define __TIMER_H_

#include <stddef.h>

/**
 * @brief Read the `mtime` register.
 *
 * Rust equivalent: `timer::get_time`.
 *
 * @return Current `mtime` value, in ticks.
 */
size_t timer_get_time();

/**
 * @brief Get current time in milliseconds.
 *
 * Rust equivalent: `timer::get_time_ms`.
 *
 * @return Milliseconds since boot.
 */
size_t timer_get_time_ms();

/**
 * @brief Set the next timer interrupt.
 *
 * Rust equivalent: `timer::set_next_trigger`. Programs
 * `mtimecmp = now + clock_freq / TICKS_PER_SEC`, i.e. the next interrupt fires
 * after one time slice (10 ms by default).
 */
void timer_set_next_trigger();

#endif /* __TIMER_H_ */
