/**
 * @file map.c
 * @brief An ordered map from `VirtPageNum` to `FrameTracker`.
 */

#include "map.h"
#include "../panic.h"

#include <stddef.h>

void vpn_frame_map_init(VpnFrameMap* map) {
    vec_init(&map->entries, sizeof(VpnFrameEntry));
}

/**
 * @brief Return the first index whose key is >= @p key .
 *
 * Binary search; O(log n).
 */
static usize vpn_frame_map_lower_bound(const VpnFrameMap* map, VirtPageNum key) {
    usize low = 0;
    usize high = vec_len(&map->entries);
    while (low < high) {
        usize mid = low + (high - low) / 2;
        const VpnFrameEntry* entry = (const VpnFrameEntry*)vec_get(&map->entries, mid);
        if (entry->key.value < key.value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

void vpn_frame_map_insert(VpnFrameMap* map, VirtPageNum key, FrameTracker value) {
    usize idx = vpn_frame_map_lower_bound(map, key);
    usize len = vec_len(&map->entries);
    if (idx < len) {
        VpnFrameEntry* existing = (VpnFrameEntry*)vec_get_mut(&map->entries, idx);
        if (existing->key.value == key.value) {
            // BTreeMap::insert drops the old value
            frame_dealloc(existing->value.ppn);
            existing->value = value;
            return;
        }
    }

    // push a copy at the tail, then shift [idx, len) one slot up
    VpnFrameEntry new_entry;
    new_entry.key = key;
    new_entry.value = value;
    if (!vec_push(&map->entries, &new_entry)) {
        panic("vpn_frame_map_insert: out of memory");
    }
    for (usize i = len; i > idx; i--) {
        VpnFrameEntry* dst = (VpnFrameEntry*)vec_get_mut(&map->entries, i);
        const VpnFrameEntry* src = (const VpnFrameEntry*)vec_get(&map->entries, i - 1);
        *dst = *src;
    }
    VpnFrameEntry* dst = (VpnFrameEntry*)vec_get_mut(&map->entries, idx);
    *dst = new_entry;
}

bool vpn_frame_map_remove(VpnFrameMap* map, VirtPageNum key, FrameTracker* out) {
    usize idx = vpn_frame_map_lower_bound(map, key);
    usize len = vec_len(&map->entries);
    if (idx >= len) {
        return false;
    }
    VpnFrameEntry* existing = (VpnFrameEntry*)vec_get_mut(&map->entries, idx);
    if (existing->key.value != key.value) {
        return false;
    }
    if (out != NULL) {
        *out = existing->value;
    }
    for (usize i = idx; i + 1 < len; i++) {
        VpnFrameEntry* dst = (VpnFrameEntry*)vec_get_mut(&map->entries, i);
        const VpnFrameEntry* src = (const VpnFrameEntry*)vec_get(&map->entries, i + 1);
        *dst = *src;
    }
    map->entries.len--;
    return true;
}

bool vpn_frame_map_get(const VpnFrameMap* map, VirtPageNum key, FrameTracker* out) {
    usize idx = vpn_frame_map_lower_bound(map, key);
    usize len = vec_len(&map->entries);
    if (idx >= len) {
        return false;
    }
    const VpnFrameEntry* existing = (const VpnFrameEntry*)vec_get(&map->entries, idx);
    if (existing->key.value != key.value) {
        return false;
    }
    if (out != NULL) {
        *out = existing->value;
    }
    return true;
}

usize vpn_frame_map_len(const VpnFrameMap* map) {
    return vec_len(&map->entries);
}

const VpnFrameEntry* vpn_frame_map_get_entry(const VpnFrameMap* map, usize idx) {
    return (const VpnFrameEntry*)vec_get(&map->entries, idx);
}

void vpn_frame_map_free(VpnFrameMap* map) {
    usize len = vec_len(&map->entries);
    for (usize i = 0; i < len; i++) {
        const VpnFrameEntry* entry = (const VpnFrameEntry*)vec_get(&map->entries, i);
        frame_dealloc(entry->value.ppn);
    }
    vec_free(&map->entries);
}
