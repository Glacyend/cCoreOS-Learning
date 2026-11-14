/**
 * @file mm.h
 * @brief Memory management implementation.
 *
 * SV39 page-based virtual-memory architecture for RV64 systems, and everything
 * about memory management, like `FrameAllocator`, `PageTable`, `MapArea` and
 * `MemorySet`, is implemented here.
 *
 * Every task or process has a `memory_set` to control its virtual memory.
 */

#ifndef __MM_H_
#define __MM_H_

/**
 * @brief Initiate `heap_allocator`, `frame_allocator` and `KERNEL_SPACE`.
 *
 * Rust equivalent: `mm::init`. Calls, in order:
 *
 *   1. `init_heap()` - install the kernel heap allocator
 *   2. `init_frame_allocator()` - set up the physical frame allocator
 *   3. `init_kernel_space()` - build the kernel `MemorySet`
 *   4. `memory_set_activate()` - load `satp` and flush the TLB
 *
 * Must be called once, after `clear_bss()` and before `trap_init()`.
 */
void mm_init();

#endif
