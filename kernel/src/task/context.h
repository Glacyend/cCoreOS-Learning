/**
 * @file context.h
 * @brief Implementation of `TaskContext`.
 */

#ifndef __TASK_CONTEXT_H_
#define __TASK_CONTEXT_H_

#include "../config.h"

/**
 * @brief Task context structure containing some registers.
 *
 * Rust equivalent: `TaskContext`. `#[repr(C)]` maps to a plain C struct; the
 * layout must match `switch.S`:
 *
 * ```text
 * offset 0      ra
 * offset 8      sp
 * offset 16+8n  s[n], n = 0..11
 * ```
 *
 * Total size: 14 * 8 = 112 bytes.
 */
typedef struct {
    /** @brief Return address (e.g. `__restore`) of `__switch` ASM function. */
    usize ra;

    /** @brief Kernel stack pointer of app. */
    usize sp;

    /** @brief Callee saved registers: `s0..s11`. */
    usize s[12];
} TaskContext;

/**
 * @brief Init task context.
 *
 * Rust equivalent: `TaskContext::zero_init`.
 */
TaskContext task_context_zero_init();

/**
 * @brief Set `TaskContext`.
 *
 * Rust equivalent: `TaskContext::goto_trap_return`. Fills `ra` with the
 * address of `trap_return` and `sp` with `kstack_ptr`, and zeroes the
 * callee-saved registers.
 *
 * ```text
 * TaskContext {
 *     __restore ASM function: trap_return,
 *     sp: kstack_ptr,
 *     s: s_0..12,
 * }
 * ```
 *
 * @param kstack_ptr Kernel stack pointer, typically the address of the
 *                   `TrapContext` pushed by `loader_init_app_cx`.
 */
TaskContext task_context_goto_trap_return(usize kstack_ptr);

#endif /* __TASK_CONTEXT_H_ */
