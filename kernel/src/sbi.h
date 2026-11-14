/**
 * @file sbi.h
 * @brief SBI call wrappers.
 */

#ifndef __SBI_H_
#define __SBI_H_

#include <stddef.h>

/**
 * @brief Use SBI call to put char in console (qemu uart handler).
 *
 * Rust equivalent: `sbi::console_putchar`. Uses the legacy console extension
 * `SBI_EXT_LEGACY_CONSOLE_PUTCHAR` (`a7 = 0x01`), which `sbi_rt` marks as
 * deprecated but OpenSBI still supports.
 *
 * @param c Character (expected to be a byte; only ASCII is correct).
 */
void sbi_console_putchar(size_t c);

/**
 * @brief Use SBI call to set timer.
 *
 * Rust equivalent: `sbi::set_timer`. Uses the SBI v0.2+ TIME extension
 * (`a7 = "TIME"`, `a6 = 0`).
 *
 * @param timer Absolute `mtime` tick at which the timer should fire.
 */
void sbi_set_timer(size_t timer);

/**
 * @brief Use SBI call to shutdown the kernel.
 *
 * Rust equivalent: `sbi::shutdown`. Uses the SBI v0.2+ SRST extension
 * (`a7 = "SRST"`, `a6 = 0`).
 *
 * @param failure If true, report `SystemFailure`; otherwise `NoReason`.
 */
[[noreturn]] void sbi_shutdown(bool failure);

#endif /* __SBI_H_ */
