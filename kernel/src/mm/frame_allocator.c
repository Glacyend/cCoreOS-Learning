/**
 * @file frame_allocator.c
 * @brief Implementation of `FrameAllocator` which controls all the frames in
 *        the operating system.
 */

#include "frame_allocator.h"
#include "../boards/qemu.h"
#include "../panic.h"
#include "vec.h"

/**
 * @brief A stack-based frame allocator with a recycled list.
 *
 * Rust equivalent: `StackFrameAllocator`.
 */
typedef struct {
    usize current;
    usize end;
    Vector recycled;
} StackFrameAllocator;

static StackFrameAllocator global_frame_allocator;

/**
 * @brief Initialize @p allocator to manage `[left, right)`.
 *
 * Rust equivalent: `StackFrameAllocator::init`.
 */
static void stack_frame_allocator_init(
    StackFrameAllocator* allocator, PhysPageNum left, PhysPageNum right
) {
    allocator->current = left.value;
    allocator->end = right.value;
    vec_init(&allocator->recycled, sizeof(usize));
}

/**
 * @brief Allocate one frame, preferring the recycled list.
 *
 * Rust equivalent: `FrameAllocator::alloc`.
 */
static bool stack_frame_allocator_alloc(StackFrameAllocator* allocator, PhysPageNum* out) {
    usize ppn;
    if (vec_pop(&allocator->recycled, &ppn)) {
        *out = phys_page_num_from_usize(ppn);
        return true;
    } else if (allocator->current == allocator->end) {
        return false;
    } else {
        allocator->current++;
        *out = phys_page_num_from_usize(allocator->current - 1);
        return true;
    }
}

/**
 * @brief Return @p ppn to the recycled list.
 *
 * Rust equivalent: `FrameAllocator::dealloc`.
 */
static void stack_frame_allocator_dealloc(StackFrameAllocator* allocator, PhysPageNum ppn) {
    usize ppn_value = ppn.value;
    // validity check
    if (ppn_value >= allocator->current) {
        panic("Frame ppn=%lX has not been allocated!", (unsigned long)ppn_value);
    }
    for (usize i = 0; i < vec_len(&allocator->recycled); i++) {
        const usize* elem = (const usize*)vec_get(&allocator->recycled, i);
        if (*elem == ppn_value) {
            panic("Frame ppn=%lX has not been allocated!", (unsigned long)ppn_value);
        }
    }
    // recycle
    if (!vec_push(&allocator->recycled, &ppn_value)) {
        panic("frame_dealloc: out of memory");
    }
}

/**
 * @brief Create a tracker for @p ppn and zero the page.
 *
 * Rust equivalent: `FrameTracker::new`.
 */
static FrameTracker frame_tracker_new(PhysPageNum ppn) {
    // page cleaning
    uint8_t* bytes_array = phys_page_num_get_bytes_array(ppn);
    for (usize i = 0; i < PAGE_SIZE; i++) {
        bytes_array[i] = 0;
    }
    FrameTracker tracker;
    tracker.ppn = ppn;
    return tracker;
}

extern char ekernel[];

void init_frame_allocator() {
    PhysAddr ekernel_pa = phys_addr_from_usize((usize)ekernel);
    PhysAddr memory_end_pa = phys_addr_from_usize(MEMORY_END);
    stack_frame_allocator_init(
        &global_frame_allocator, phys_addr_ceil(ekernel_pa), phys_addr_floor(memory_end_pa)
    );
}

bool frame_alloc(FrameTracker* out) {
    PhysPageNum ppn;
    if (!stack_frame_allocator_alloc(&global_frame_allocator, &ppn)) {
        return false;
    }
    *out = frame_tracker_new(ppn);
    return true;
}

void frame_dealloc(PhysPageNum ppn) {
    stack_frame_allocator_dealloc(&global_frame_allocator, ppn);
}
