/**
 * @file context.h
 * @brief Implementation of `TrapContext`.
 */

#ifndef __TRAP_CONTEXT_H_
#define __TRAP_CONTEXT_H_

#include <stddef.h>

/**
 * @brief Trap context structure containing `sstatus`, `sepc` and registers.
 *
 * Rust equivalent: `TrapContext`. `#[repr(C)]` maps to a plain C struct; the
 * layout is load-bearing because `trap.S` uses hard-coded byte offsets:
 *
 * ```text
 * offset 0      x[0]
 * offset 1*8    x[1]
 * ...
 * offset 31*8   x[31]
 * offset 32*8   sstatus
 * offset 33*8   sepc
 * offset 34*8   kernel_satp
 * offset 35*8   kernel_sp
 * offset 36*8   trap_handler
 * ```
 *
 * Total size: 37 * 8 = 296 bytes.
 */
typedef struct {
    /** @brief General regs[0..31]. */
<<<<<<< Updated upstream
    usize x[32];

    /** @brief CSR sstatus. */
    usize sstatus;

    /** @brief CSR sepc. */
    usize sepc;

    /** @brief Addr of `PageTable`. */
    usize kernel_satp;

    /** @brief Kernel stack. */
    usize kernel_sp;

    /** @brief Addr of `trap_handler` function. */
    usize trap_handler;
=======
    size_t x[32];

    /** @brief CSR sstatus. */
    size_t sstatus;

    /** @brief CSR sepc. */
    size_t sepc;

    /** @brief Addr of `PageTable`. */
    size_t kernel_satp;

    /** @brief Kernel stack. */
    size_t kernel_sp;

    /** @brief Addr of `trap_handler` function. */
    size_t trap_handler;
>>>>>>> Stashed changes
} TrapContext;

/**
 * @brief Set stack pointer to x2 reg (sp).
 *
 * Rust equivalent: `TrapContext::set_sp`.
 */
<<<<<<< Updated upstream
void trap_context_set_sp(TrapContext* ctx, usize sp);
=======
void trap_context_set_sp(TrapContext* ctx, size_t sp);
>>>>>>> Stashed changes

/**
 * @brief Init app context.
 *
 * Reads the current `sstatus`, sets `SPP = User`, zeroes all general
 * registers, and fills `sepc`, `kernel_satp`, `kernel_sp`, `trap_handler`. The
 * user stack pointer is written into `x[2]`.
 *
 * Rust equivalent: `TrapContext::app_init_context`.
 *
 * @param entry        Entry point of the app, written into `sepc`.
 * @param sp           User stack pointer, written into `x[2]`.
 * @param kernel_satp  `satp` of the kernel page table.
 * @param kernel_sp    Kernel stack pointer.
 * @param trap_handler Address of `trap_handler`.
 * @return The initialized `TrapContext`.
 */
TrapContext trap_context_app_init_context(
<<<<<<< Updated upstream
    usize entry, usize sp, usize kernel_satp, usize kernel_sp, usize trap_handler
=======
    size_t entry, size_t sp, size_t kernel_satp, size_t kernel_sp, size_t trap_handler
>>>>>>> Stashed changes
);

#endif /* __TRAP_CONTEXT_H_ */
