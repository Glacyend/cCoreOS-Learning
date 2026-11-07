/**
 * @file trap.c
 * @brief Trap handling.
 */

#include "trap.h"
#include "../logging.h"
#include "../panic.h"
#include "../syscall/syscall.h"

// CSR read helpers.

static inline usize read_stval() {
    usize v;
    __asm__ volatile("csrr %0, stval" : "=r"(v));
    return v;
}

static inline usize read_scause() {
    usize v;
    __asm__ volatile("csrr %0, scause" : "=r"(v));
    return v;
}

#define SCAUSE_INTERRUPT (1ul << 63)
#define SCAUSE_CODE_MASK 0xFFul

#define EXC_ILLEGAL_INSTR 2
#define EXC_LOAD_FAULT 5
#define EXC_STORE_FAULT 7
#define EXC_USER_ECALL 8
#define EXC_S_ECALL 9

extern char __alltraps[];

void trap_init() {
    usize addr = (usize)__alltraps;
    __asm__ volatile("csrw stvec, %0" : : "r"(addr));
}

TrapContext* trap_handler(TrapContext* cx) {
    usize scause = read_scause();
    usize stval = read_stval();

    if ((scause & SCAUSE_INTERRUPT) != 0) {
        panic("[kernel] interrupt 0x%lX not yet implemented", (unsigned long)scause);
    }

    usize code = scause & SCAUSE_CODE_MASK;
    switch (code) {
        case EXC_USER_ECALL: {
            cx->sepc += 4;
            cx->x[10] = (usize)syscall_dispatch(cx->x[17], cx->x[10], cx->x[11], cx->x[12]);
            break;
        }
        case EXC_S_ECALL: {
            trace("[kernel] Supervisor ecall received, trap framework works.");
            cx->sepc += 4;
            break;
        }
        case EXC_LOAD_FAULT:
        case EXC_STORE_FAULT: {
            panic(
                "[kernel] Fault at 0x%lX, sepc = 0x%lX",
                (unsigned long)stval,
                (unsigned long)cx->sepc
            );
            break;
        }
        case EXC_ILLEGAL_INSTR: {
            panic("[kernel] Illegal instruction at sepc = 0x%lX", (unsigned long)cx->sepc);
            break;
        }
        default: {
            panic(
                "[kernel] Unsupported trap: scause = 0x%lX, stval = 0x%lX",
                (unsigned long)scause,
                (unsigned long)stval
            );
            break;
        }
    }

    return cx;
}
