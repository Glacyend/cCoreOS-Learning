/**
 * @file task.c
 * @brief Batch scheduler: run every embedded app in order.
 *
 * Each app gets a fixed kernel stack and user stack. Before jumping to U-mode,
 * a `TrapContext` is pushed onto the kernel stack and `__restore` is called
 * with its address.
 */

#include "task.h"
#include "../config.h"
#include "../loader.h"
#include "../logging.h"
#include "../sbi.h"
#include "../trap/context.h"

/**
 * @brief One kernel stack per app, 4 KiB-aligned.
 */
typedef struct {
    alignas(4096) char data[KERNEL_STACK_SIZE];
} KernelStack;

/**
 * @brief One user stack per app, 4 KiB-aligned.
 */
typedef struct {
    alignas(4096) char data[USER_STACK_SIZE];
} UserStack;

static KernelStack global_kernel_stack[MAX_APP_NUM];
[[maybe_unused]] static UserStack global_user_stack[MAX_APP_NUM];

static usize global_current_app = 0;

static usize kernel_stack_top(usize app_id) {
    return (usize)&global_kernel_stack[app_id].data[KERNEL_STACK_SIZE];
}

[[maybe_unused]] static usize user_stack_top(usize app_id) {
    return (usize)&global_user_stack[app_id].data[USER_STACK_SIZE];
}

/**
 * @brief Push @p cx into the app's kernel stack; return its address.
 *
 * @param app_id The app ID.
 * @param cx     The trap context to push.
 * @return The address of the pushed trap context.
 */
static usize push_trap_context(usize app_id, const TrapContext* cx) {
    usize sp = kernel_stack_top(app_id);
    usize addr = sp - sizeof(TrapContext);
    *(TrapContext*)addr = *cx;
    return addr;
}

/**
 * @brief Initialize a `TrapContext` for entering U-mode.
 *
 * Reads `sstatus`, sets `SPP = User` (bit 8 cleared), zeros all registers,
 * sets `sepc = entry` and `x[2] = sp`.
 *
 * @param cx    The trap context to initialize.
 * @param entry The entry point of the U-mode program.
 * @param sp    The stack pointer of the U-mode program.
 */
static void init_trap_context(TrapContext* cx, usize entry, usize sp) {
    usize sstatus;
    __asm__ volatile("csrr %0, sstatus" : "=r"(sstatus));
    sstatus &= ~(1ul << 8);

    for (int i = 0; i < 32; i++) {
        cx->x[i] = 0;
    }
    cx->sstatus = sstatus;
    cx->sepc = entry;
    cx->x[2] = sp;
}

// Provided by trap.S.
[[noreturn]] extern void __restore(usize cx_addr);

[[noreturn]] void task_run_next_app() {
    usize num_app = loader_get_num_app();
    if (global_current_app >= num_app) {
        info("[kernel] All application completed.");
        sbi_shutdown(false);
    }

    usize app_id = global_current_app++;
    loader_load_app(app_id);

    TrapContext cx;
    init_trap_context(&cx, APP_BASE_ADDRESS, user_stack_top(app_id));
    usize cx_addr = push_trap_context(app_id, &cx);

    info(
        "[kernel] Running app %lu at 0x%lX", (unsigned long)app_id, (unsigned long)APP_BASE_ADDRESS
    );

    __restore(cx_addr);
}
