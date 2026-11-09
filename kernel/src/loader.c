/**
 * @file loader.c
 * @brief Load user applications embedded in the kernel image.
 */

#include "loader.h"
#include "panic.h"
#include "trap/context.h"

// Symbols produced by embed_apps.S
extern char _num_app[];

typedef struct {
    alignas(4096) char data[KERNEL_STACK_SIZE];
} KernelStack;

static KernelStack global_kernel_stack[MAX_APP_NUM];

static usize kernel_stack_top(usize app_id) {
    return (usize)&global_kernel_stack[app_id].data[KERNEL_STACK_SIZE];
}

static usize user_stack_top(usize app_id) {
    return APP_BASE_ADDRESS + (app_id + 1) * APP_SIZE_LIMIT;
}

usize loader_get_num_app() {
    return *(const usize*)_num_app;
}

void loader_load_app(usize app_id) {
    usize num_app = loader_get_num_app();
    assert_panic(app_id < num_app);

    const usize* table = (const usize*)_num_app;
    usize src_start = table[1 + app_id];
    usize src_end = table[2 + app_id];
    usize src_len = src_end - src_start;

    usize dst = APP_BASE_ADDRESS + app_id * APP_SIZE_LIMIT;

    for (usize i = 0; i < APP_SIZE_LIMIT; i++) {
        *(volatile char*)(dst + i) = 0;
    }

    for (usize i = 0; i < src_len; i++) {
        *(volatile char*)(dst + i) = *(const char*)(src_start + i);
    }

    __asm__ volatile("fence.i");
}

void loader_load_apps() {
    usize num_app = loader_get_num_app();
    for (usize i = 0; i < num_app; i++) {
        loader_load_app(i);
    }
}

usize loader_init_app_cx(usize app_id) {
    TrapContext cx;

    usize sstatus;
    __asm__ volatile("csrr %0, sstatus" : "=r"(sstatus));
    sstatus &= ~(1ul << 8);
    sstatus |= (1ul << 5);

    for (int i = 0; i < 32; i++) {
        cx.x[i] = 0;
    }
    cx.sstatus = sstatus;
    cx.sepc = APP_BASE_ADDRESS + app_id * APP_SIZE_LIMIT;
    cx.x[2] = user_stack_top(app_id);

    usize sp = kernel_stack_top(app_id);
    usize addr = sp - sizeof(TrapContext);
    *(TrapContext*)addr = cx;
    return addr;
}
