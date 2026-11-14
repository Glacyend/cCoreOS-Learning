/**
 * @file mm.c
 * @brief Memory management implementation.
 */

#include "mm.h"
#include "frame_allocator.h"
#include "heap_allocator.h"
#include "memory_set.h"

void mm_init() {
    init_heap();
    init_frame_allocator();
    init_kernel_space();
    memory_set_activate(kernel_space());
}
