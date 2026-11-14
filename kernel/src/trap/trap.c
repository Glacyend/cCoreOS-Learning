/**
 * @file trap.c
 * @brief Trap handling functionality.
 */

#include "trap.h"
#include "../config.h"
#include "../logging.h"
#include "../panic.h"
#include "../syscall/syscall.h"
#include "../task/task.h"
#include "../timer.h"
#include "context.h"

/**
 * @brief Top bit of `scause`: 1 = interrupt, 0 = exception.
 */
<<<<<<< Updated upstream
static const usize SCAUSE_INTERRUPT_BIT = 1ul << 63;
=======
static const size_t SCAUSE_INTERRUPT_BIT = 1ul << 63;
>>>>>>> Stashed changes

/**
 * @brief Bit mask for the exception/interrupt code in `scause`.
 */
<<<<<<< Updated upstream
static const usize SCAUSE_CODE_MASK = 0xFFul;

// Interrupt codes (bit 63 set).
static const usize INT_SUPERVISOR_SOFTWARE = 1;
static const usize INT_SUPERVISOR_TIMER = 5;
static const usize INT_SUPERVISOR_EXTERNAL = 9;

// Exception codes (bit 63 clear).
static const usize EXC_INSTRUCTION_ADDRESS_MISALIGNED = 0;
static const usize EXC_INSTRUCTION_ACCESS_FAULT = 1;
static const usize EXC_ILLEGAL_INSTRUCTION = 2;
static const usize EXC_BREAKPOINT = 3;
static const usize EXC_LOAD_ADDRESS_MISALIGNED = 4;
static const usize EXC_LOAD_FAULT = 5;
static const usize EXC_STORE_ADDRESS_MISALIGNED = 6;
static const usize EXC_STORE_FAULT = 7;
static const usize EXC_USER_ENV_CALL = 8;
static const usize EXC_SUPERVISOR_ENV_CALL = 9;
static const usize EXC_INSTRUCTION_PAGE_FAULT = 12;
static const usize EXC_LOAD_PAGE_FAULT = 13;
static const usize EXC_STORE_PAGE_FAULT = 14;
=======
static const size_t SCAUSE_CODE_MASK = 0xFFul;

// Interrupt codes (bit 63 set).
// static const size_t INT_SUPERVISOR_SOFTWARE = 1;
static const size_t INT_SUPERVISOR_TIMER = 5;
// static const size_t INT_SUPERVISOR_EXTERNAL = 9;

// Exception codes (bit 63 clear).
// static const size_t EXC_INSTRUCTION_ADDRESS_MISALIGNED = 0;
// static const size_t EXC_INSTRUCTION_ACCESS_FAULT = 1;
static const size_t EXC_ILLEGAL_INSTRUCTION = 2;
// static const size_t EXC_BREAKPOINT = 3;
// static const size_t EXC_LOAD_ADDRESS_MISALIGNED = 4;
static const size_t EXC_LOAD_FAULT = 5;
// static const size_t EXC_STORE_ADDRESS_MISALIGNED = 6;
static const size_t EXC_STORE_FAULT = 7;
static const size_t EXC_USER_ENV_CALL = 8;
// static const size_t EXC_SUPERVISOR_ENV_CALL = 9;
// static const size_t EXC_INSTRUCTION_PAGE_FAULT = 12;
static const size_t EXC_LOAD_PAGE_FAULT = 13;
static const size_t EXC_STORE_PAGE_FAULT = 14;
>>>>>>> Stashed changes

extern char __alltraps[];
extern char __restore[];

<<<<<<< Updated upstream
static usize read_scause() {
    usize value;
=======
static size_t read_scause() {
    size_t value;
>>>>>>> Stashed changes
    __asm__ volatile("csrr %0, scause" : "=r"(value));
    return value;
}

<<<<<<< Updated upstream
static usize read_stval() {
    usize value;
=======
static size_t read_stval() {
    size_t value;
>>>>>>> Stashed changes
    __asm__ volatile("csrr %0, stval" : "=r"(value));
    return value;
}

static void set_kernel_trap_entry() {
<<<<<<< Updated upstream
    __asm__ volatile("csrw stvec, %0" : : "r"((usize)trap_from_kernel));
=======
    __asm__ volatile("csrw stvec, %0" : : "r"((size_t)trap_from_kernel));
>>>>>>> Stashed changes
}

static void set_user_trap_entry() {
    __asm__ volatile("csrw stvec, %0" : : "r"(TRAMPOLINE));
}

void trap_init() {
    set_kernel_trap_entry();
}

void trap_enable_timer_interrupt() {
    __asm__ volatile("csrs sie, %0" : : "r"(1ul << 5));
}

[[noreturn]] void trap_handler() {
    set_kernel_trap_entry();
    TrapContext* cx = task_current_trap_cx();
<<<<<<< Updated upstream
    usize scause = read_scause();
    usize stval = read_stval();

    bool is_interrupt = (scause & SCAUSE_INTERRUPT_BIT) != 0;
    usize code = scause & (SCAUSE_CODE_MASK);
=======
    size_t scause = read_scause();
    size_t stval = read_stval();

    bool is_interrupt = (scause & SCAUSE_INTERRUPT_BIT) != 0;
    size_t code = scause & (SCAUSE_CODE_MASK);
>>>>>>> Stashed changes

    if (is_interrupt) {
        switch (code) {
            case INT_SUPERVISOR_TIMER: {
                timer_set_next_trigger();
                task_suspend_current_and_run_next();
                break;
            }
            default: {
<<<<<<< Updated upstream
                panic(
                    "Unsupported trap scause = 0x%lX, stval = 0x%lX!",
                    (unsigned long)scause,
                    (unsigned long)stval
                );
=======
                panic("Unsupported trap scause = 0x%zX, stval = 0x%zX!", scause, stval);
>>>>>>> Stashed changes
            }
        }
    } else {
        switch (code) {
            case EXC_USER_ENV_CALL: {
                cx->sepc += 4;
<<<<<<< Updated upstream
                cx->x[10] = (usize)syscall_dispatch(cx->x[17], cx->x[10], cx->x[11], cx->x[12]);
=======
                cx->x[10] = (size_t)syscall_dispatch(cx->x[17], cx->x[10], cx->x[11], cx->x[12]);
>>>>>>> Stashed changes
                break;
            }
            case EXC_STORE_FAULT:
            case EXC_STORE_PAGE_FAULT:
            case EXC_LOAD_FAULT:
            case EXC_LOAD_PAGE_FAULT: {
                warn(
<<<<<<< Updated upstream
                    "[kernel] PageFault in application, bad addr = 0x%lX, bad instruction = 0x%lX, "
                    "kernel killed it.",
                    (unsigned long)stval,
                    (unsigned long)cx->sepc
=======
                    "[kernel] PageFault in application, bad addr = 0x%zX, bad instruction = 0x%zX, "
                    "kernel killed it.",
                    stval,
                    cx->sepc
>>>>>>> Stashed changes
                );
                task_exit_current_and_run_next();
            }
            case EXC_ILLEGAL_INSTRUCTION: {
                warn("[kernel] IllegalInstruction in application, kernel killed it.");
                task_exit_current_and_run_next();
            }
            default: {
<<<<<<< Updated upstream
                panic(
                    "Unsupported trap scause = 0x%lX, stval = 0x%lX!",
                    (unsigned long)scause,
                    (unsigned long)stval
                );
=======
                panic("Unsupported trap scause = 0x%zX, stval = 0x%zX!", scause, stval);
>>>>>>> Stashed changes
            }
        }
    }

    trap_return();
}

[[noreturn]] void trap_return() {
    set_user_trap_entry();
<<<<<<< Updated upstream
    usize trap_cx_ptr = TRAP_CONTEXT;
    usize user_satp = task_current_user_token();
    usize restore_va = (usize)__restore - (usize)__alltraps + TRAMPOLINE;

    register usize a0 __asm__("a0") = trap_cx_ptr;
    register usize a1 __asm__("a1") = user_satp;
=======
    size_t trap_cx_ptr = TRAP_CONTEXT;
    size_t user_satp = task_current_user_token();
    size_t restore_va = (size_t)__restore - (size_t)__alltraps + TRAMPOLINE;

    register size_t a0 __asm__("a0") = trap_cx_ptr;
    register size_t a1 __asm__("a1") = user_satp;
>>>>>>> Stashed changes
    __asm__ volatile("fence.i");
    __asm__ volatile("jr %0" : : "r"(restore_va), "r"(a0), "r"(a1) : "memory");
    __builtin_unreachable();
}

[[noreturn]] void trap_from_kernel() {
    panic("a trap from kernel");
}
