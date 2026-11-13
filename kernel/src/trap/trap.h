/**
 * @file trap.h
 * @brief Trap handling functionality.
 *
 * For cCoreOS, we have a single trap entry point, namely `__alltraps`. At
 * initialization in `trap_init()`, we set the `stvec` CSR to point to it.
 *
 * All traps go through `__alltraps`, which is defined in `trap.S`. The
 * assembly language code does just enough work restore the kernel space
 * context, ensuring that C code safely runs, and transfers control to
 * `trap_handler`.
 *
 * It then calls different functionality based on what exactly the exception
 * was. For example, timer interrupts trigger task preemption, and syscalls go
 * to `syscall_dispatch`.
 */

#ifndef __TRAP_H_
#define __TRAP_H_

/**
 * @brief Initialize CSR `stvec` at the entry of `__alltraps`.
 *
 * Rust equivalent: `trap::init`.
 */
void trap_init();

/**
 * @brief Enable timer interrupt in `sie` CSR.
 *
 * Rust equivalent: `trap::enable_timer_interrupt`.
 */
void trap_enable_timer_interrupt();

/**
 * @brief Handle an interrupt, exception, or system call from user space.
 *
 * Rust equivalent: `trap::trap_handler`.
 */
[[noreturn]] void trap_handler();

/**
 * @brief Set the new address of `__restore` asm function in `TRAMPOLINE` page.
 *
 * Set the register `a0` to `trap_cx_ptr`, register `a1` to physical address of
 * user page table.
 *
 * Rust equivalent: `trap::trap_return`.
 */
[[noreturn]] void trap_return();

/**
 * @brief Unimplement: traps/interrupts/exceptions from kernel mode.
 *
 * Todo: I/O device
 *
 * Rust equivalent: `trap::trap_from_kernel`.
 */
[[noreturn]] void trap_from_kernel();

#endif /* __TRAP_H_ */
