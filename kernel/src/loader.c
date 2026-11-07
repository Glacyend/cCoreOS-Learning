/**
 * @file loader.c
 * @brief Load user applications embedded in the kernel image.
 */

#include "loader.h"
#include "panic.h"

// Symbols produced by embed_apps.S
extern char _num_app[];
extern char app_0_start[];
extern char app_0_end[];

static inline usize read_usize(const char* ptr) {
    return *(const usize*)ptr;
}

usize loader_get_num_app() {
    return read_usize(_num_app);
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
