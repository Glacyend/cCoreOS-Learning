/**
 * @file sbi.c
 * @brief SBI call wrappers.
 */

#include "sbi.h"
#include "config.h"

// SBI extension IDs (v0.2+).
#define SBI_EXT_TIME 0x54494D45ul
#define SBI_EXT_SRST 0x53525354ul
#define SBI_EXT_LEGACY_CONSOLE_PUTCHAR 0x01ul

// SRST reset types/reasons.
#define SBI_SRST_RESET_TYPE_SHUTDOWN 0ul
#define SBI_SRST_RESET_TYPE_COLD_REBOOT 1ul
#define SBI_SRST_RESET_REASON_NO_REASON 0ul
#define SBI_SRST_RESET_REASON_SYSTEM_FAILURE 1ul

/**
 * @brief Issue an `ecall` with the given SBI extension and function.
 *
 * Rust equivalent: the body of every `sbi_rt` wrapper, which issues `ecall`
 * with the extension ID in `a7`, the function ID in `a6`, and arguments in
 * `a0..a5`.
 *
 * @param ext  SBI extension ID (placed in `a7`).
 * @param fid  Function ID within the extension (placed in `a6`).
 * @param arg0 First argument (`a0`).
 * @param arg1 Second argument (`a1`).
 * @param arg2 Third argument (`a2`).
 * @return Value returned in `a0`.
 */
static inline long sbi_call(size_t ext, size_t fid, size_t arg0, size_t arg1, size_t arg2) {
    register size_t a0 __asm__("a0") = arg0;
    register size_t a1 __asm__("a1") = arg1;
    register size_t a2 __asm__("a2") = arg2;
    register size_t a6 __asm__("a6") = fid;
    register size_t a7 __asm__("a7") = ext;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a6), "r"(a7) : "memory");
    return (long)a0;
}

void sbi_console_putchar(size_t c) {
    sbi_call(SBI_EXT_LEGACY_CONSOLE_PUTCHAR, 0, c, 0, 0);
}

void sbi_set_timer(size_t timer) {
    sbi_call(SBI_EXT_TIME, 0, timer, 0, 0);
}

[[noreturn]] void sbi_shutdown(bool failure) {
    size_t reason = failure ? SBI_SRST_RESET_REASON_SYSTEM_FAILURE : SBI_SRST_RESET_REASON_NO_REASON;
    sbi_call(SBI_EXT_SRST, 0, SBI_SRST_RESET_TYPE_SHUTDOWN, reason, 0);
    // unreachable
    __builtin_unreachable();
}
