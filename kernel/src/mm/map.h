/**
 * @file map.h
 * @brief An ordered map from `VirtPageNum` to `FrameTracker`.
 *
 * Rust equivalent: `alloc::collections::BTreeMap<VirtPageNum, FrameTracker>`.
 *
 * Backed by a sorted array of key-value pairs. `insert` and `remove` use
 * binary search to locate the key, then `memmove` the tail. For
 * `MapArea::data_frames`, which holds at most a few hundred pages, the O(n)
 * move is negligible and the sorted order matches the Rust `BTreeMap`
 * semantics.
 */

#ifndef __MM_MAP_H_
#define __MM_MAP_H_

#include "address.h"
#include "frame_allocator.h"
#include "vec.h"

/**
 * @brief One entry in @ref VpnFrameMap .
 */
typedef struct {
    VirtPageNum key;
    FrameTracker value;
} VpnFrameEntry;

/**
 * @brief An ordered map from @ref VirtPageNum to @ref FrameTracker .
 *
 * Rust equivalent: `BTreeMap<VirtPageNum, FrameTracker>`.
 */
typedef struct {
    Vector entries;
} VpnFrameMap;

/**
 * @brief Initialize an empty map.
 *
 * Rust equivalent: `BTreeMap::new`.
 */
void vpn_frame_map_init(VpnFrameMap* map);

/**
 * @brief Insert or overwrite `key`.
 *
 * If `key` already exists, the old `FrameTracker` is deallocated, matching
 * `BTreeMap::insert` dropping the old value.
 *
 * Rust equivalent: `BTreeMap::insert`.
 */
void vpn_frame_map_insert(VpnFrameMap* map, VirtPageNum key, FrameTracker value);

/**
 * @brief Remove `key`.
 *
 * On success, writes the removed value to `*out` so the caller can deallocate
 * it, mirroring the `Drop` that would run on the `FrameTracker` in Rust.
 *
 * Rust equivalent: `BTreeMap::remove`.
 *
 * @return `true` if the key existed, `false` otherwise.
 */
bool vpn_frame_map_remove(VpnFrameMap* map, VirtPageNum key, FrameTracker* out);

/**
 * @brief Look up `key`.
 *
 * Rust equivalent: `BTreeMap::get`.
 *
 * @return `true` if the key exists, `false` otherwise.
 */
bool vpn_frame_map_get(const VpnFrameMap* map, VirtPageNum key, FrameTracker* out);

/**
 * @brief Number of entries.
 *
 * Rust equivalent: `BTreeMap::len`.
 */
usize vpn_frame_map_len(const VpnFrameMap* map);

/**
 * @brief Access entry @p idx in ascending key order.
 *
 * Rust equivalent: iterating a `BTreeMap` by value.
 */
const VpnFrameEntry* vpn_frame_map_get_entry(const VpnFrameMap* map, usize idx);

/**
 * @brief Release entry `FrameTracker` and the backing buffer.
 *
 * Rust equivalent: dropping a `BTreeMap`, which drops each value and triggers
 * `FrameTracker::drop`.
 */
void vpn_frame_map_free(VpnFrameMap* map);

#endif /* __MM_MAP_H_ */
