/**
 * @file sbi.h
 * @brief SBI call wrappers.
 *
 * Thin wrappers around the SBI v0.2+ interface exposed by OpenSBI.
 * All functions issue `ecall` with the appropriate extension ID and function
 * ID in a7/a6.
 */

#ifndef __SBI_H__
#define __SBI_H__

#include "config.h"

/**
 * @brief Write one character to the console via SBI legacy extension.
 *
 * @param c Character (expected to be a byte; only ASCII is correct).
 */
void console_putchar(usize c);

/**
 * @brief Program the next timer interrupt.
 *
 * @param timer Absolute `mtime` tick at which the timer should fire.
 */
void set_timer(usize timer);

/**
 * @brief Shut down the machine via SBI SRST extension.
 *
 * @param failure If true, report `SystemFailure`; otherwise `NoReason`.
 * @return Never returns.
 */
[[noreturn]] void shutdown(int failure);

#endif /* __SBI_H__ */
