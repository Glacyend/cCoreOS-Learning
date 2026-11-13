/**
 * @file slice.h
 * @brief A borrowed byte slice, the C counterpart of `&[u8]` / `&mut [u8]`.
 */

#ifndef __MM_SLICE_H_
#define __MM_SLICE_H_

#include "../config.h"

/**
 * @brief A borrowed byte slice produced by the kernel.
 *
 * Rust equivalent: `&[u8]` or `&mut [u8]`. C has no lifetimes, so the caller
 * must keep the underlying memory alive for as long as the slice is used.
 */
typedef struct {
    uint8_t* data;
    usize len;
} ByteSlice;

#endif /* __MM_SLICE_H_ */
