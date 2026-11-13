/**
 * @file context.c
 * @brief Implementation of `TrapContext`.
 */

#include "context.h"

/**
 * @brief Supervisor Previous Privilege mode.
 *
 * Rust equivalent: `riscv::register::sstatus::SPP`. Only `SPP::User` is used
 * here, so the C version keeps the raw bit position.
 */
static const usize SPP_USER = 0;

/**
 * @brief Bit position of `SPP` in `sstatus`.
 */
static const usize SSTATUS_SPP_SHIFT = 8;

/**
 * @brief Read the `sstatus` CSR.
 *
 * Rust equivalent: `riscv::register::sstatus::read`.
 */
static usize sstatus_read() {
    usize value;
    __asm__ volatile("csrr %0, sstatus" : "=r"(value));
    return value;
}

void trap_context_set_sp(TrapContext* ctx, usize sp) {
    ctx->x[2] = sp;
}

TrapContext trap_context_app_init_context(
    usize entry, usize sp, usize kernel_satp, usize kernel_sp, usize trap_handler
) {
    usize sstatus = sstatus_read();
    sstatus = (sstatus & ~(1ul << SSTATUS_SPP_SHIFT)) | (SPP_USER << SSTATUS_SPP_SHIFT);
    TrapContext cx;
    for (usize i = 0; i < 32; i++) {
        cx.x[i] = 0;
    }
    cx.sstatus = sstatus;
    cx.sepc = entry;  // entry point of app
    cx.kernel_satp = kernel_satp;  // addr of page table
    cx.kernel_sp = kernel_sp;  // kernel stack
    cx.trap_handler = trap_handler;  // addr of trap_handler function
    trap_context_set_sp(&cx, sp);  // app's user stack pointer
    return cx;  // return initial `TrapContext` of app
}
