/**
 * @file context.c
 * @brief Implementation of `TaskContext`.
 */

#include "context.h"

/**
 * @brief Address of `trap_return`, provided by `trap.c`.
 *
 * Rust equivalent: `linker_symbol_addr!(trap::trap_return)`.
 *
 * Declared as an array so that `(usize)trap_return` is a well-defined
 * conversion of a symbol address, instead of a function pointer to integer
 * cast.
 */
extern char trap_return[];

TaskContext task_context_zero_init() {
    TaskContext context;
    context.ra = 0;
    context.sp = 0;
    for (usize i = 0; i < 12; i++) {
        context.s[i] = 0;
    }
    return context;
}

TaskContext task_context_goto_trap_return(usize kstack_ptr) {
    TaskContext context;
    context.ra = (usize)trap_return;
    context.sp = kstack_ptr;
    for (usize i = 0; i < 12; i++) {
        context.s[i] = 0;
    }
    return context;
}
