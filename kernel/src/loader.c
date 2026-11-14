/**
 * @file loader.c
 * @brief Loading user applications into memory.
 */

#include "loader.h"
#include "panic.h"

/**
 * @brief Symbol produced by `embed_apps.S`.
 *
<<<<<<< Updated upstream
 * Layout: `[num_app, start_0, ..., start_{n-1}, end_{n-1}]`, all `usize`
=======
 * Layout: `[num_app, start_0, ..., start_{n-1}, end_{n-1}]`, all `size_t`
>>>>>>> Stashed changes
 * values.
 */
extern char _num_app[];

<<<<<<< Updated upstream
usize loader_get_num_app() {
    return *(const volatile usize*)_num_app;
}

ByteSlice loader_get_app_data(usize app_id) {
    const usize* num_app_ptr = (const usize*)_num_app;
    usize num_app = loader_get_num_app();
    const usize* app_start = num_app_ptr + 1;
=======
size_t loader_get_num_app() {
    return *(const volatile size_t*)_num_app;
}

ByteSlice loader_get_app_data(size_t app_id) {
    const size_t* num_app_ptr = (const size_t*)_num_app;
    size_t num_app = loader_get_num_app();
    const size_t* app_start = num_app_ptr + 1;
>>>>>>> Stashed changes
    if (!(app_id < num_app)) {
        panic("loader_get_app_data: app_id %zu out of range (num_app= %zu)", app_id, num_app);
    }

    ByteSlice slice;
    slice.data = (uint8_t*)app_start[app_id];
    slice.len = app_start[app_id + 1] - app_start[app_id];
    return slice;
}
