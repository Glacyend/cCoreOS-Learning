/**
 * @file frame_allocator.h
 * @brief Implementation of `FrameAllocator` which controls all the frames in
 *        the operating system.
 */

#ifndef __MM_FRAME_ALLOCATOR_H_
#define __MM_FRAME_ALLOCATOR_H_

#include "address.h"

/**
 * @brief Manager for a frame which has the same lifecycle as the tracker.
 *
 * Rust `FrameTracker` implements `Drop` to return the frame when the tracker
 * goes out of scope. C has no `Drop`; the caller must call @ref frame_dealloc
 * before discarding the tracker.
 */
typedef struct {
    PhysPageNum ppn;
} FrameTracker;

/**
 * @brief Initiate the frame allocator using `ekernel` and `MEMORY_END`.
 *
 * Must be called once before any call of @ref frame_alloc or @ref
 * frame_dealloc.
 */
void init_frame_tracker();

/**
 * @brief Allocate a frame
 *
 * On success, zeroes the returned page and writes the tracker into `*out`.
 * Returns `false` when no frame is available.
 *
 * @param[out] out Receives the tracker when the return value is `true`.
 * @return `true` on success, `false` on out-of-memory.
 */
bool frame_alloc(FrameTracker* out);

/**
 * @brief Deallocate a frame.
 *
 * @param ppn Page number previously returned by @ref frame_alloc .
 */
void frame_dealloc(PhysPageNum ppn);

#endif /* __MM_FRAME_ALLOCATOR_H_ */
