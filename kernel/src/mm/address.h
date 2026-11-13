/**
 * @file address.h
 * @brief Implementation of physical and virtual address and page number.
 */

#ifndef __MM_ADDRESS_H_
#define __MM_ADDRESS_H_

#include "../config.h"

/**
 * @brief Physical address.
 */
typedef struct {
    usize value;
} PhysAddr;

/**
 * @brief Virtual address.
 */
typedef struct {
    usize value;
} VirtAddr;

/**
 * @brief Physical page number.
 */
typedef struct {
    usize value;
} PhysPageNum;

/**
 * @brief Virtual page number.
 */
typedef struct {
    usize value;
} VirtPageNum;

struct PageTableEntry;

PhysAddr phys_addr_from_usize(usize value);
VirtAddr virt_addr_from_usize(usize value);
PhysPageNum phys_page_num_from_usize(usize value);
VirtPageNum virt_page_num_from_usize(usize value);

usize phys_addr_to_usize(PhysAddr addr);
usize virt_addr_to_usize(VirtAddr addr);
usize phys_page_num_to_usize(PhysPageNum page_num);
usize virt_page_num_to_usize(VirtPageNum page_num);

VirtPageNum virt_addr_floor(VirtAddr addr);
VirtPageNum virt_addr_ceil(VirtAddr addr);
usize virt_addr_page_offset(VirtAddr addr);
bool virt_addr_aligned(VirtAddr addr);

PhysPageNum phys_addr_floor(PhysAddr addr);
PhysPageNum phys_addr_ceil(PhysAddr addr);
usize phys_addr_page_offset(PhysAddr addr);
bool phys_addr_aligned(PhysAddr addr);

VirtPageNum virt_page_num_from_virt_addr(VirtAddr addr);
VirtAddr virt_addr_from_virt_page_num(VirtPageNum page_num);
PhysPageNum phys_page_num_from_phys_addr(PhysAddr addr);
PhysAddr phys_addr_from_phys_page_num(PhysPageNum page_num);

/**
 * @brief Split the VPN into three 9-bit indices.
 *
 * @param[in]  page_num The VPN.
 * @param[out] out      `out[0] = VPN[2]`, `out[1] = VPN[1]`, `out[2] = VPN[0]`.
 */
void virt_page_num_indices(VirtPageNum page_num, usize out[3]);

/**
 * @brief `StepByOne::step` for `VirtPageNum`.
 */
void virt_page_num_step(VirtPageNum* page_num);

/**
 * @brief View the page as a 512-entry page-table array.
 *
 * The caller must ensure `PageTableEntry` is 8 bytes; the returned pointer is
 * valid for 512 entries.
 */
struct PageTableEntry* phys_page_num_get_pte_array(PhysPageNum page_num);

/**
 * @brief View the page as a 4096-byte array.
 */
uint8_t* phys_page_num_get_bytes_array(PhysPageNum page_num);

/**
 * @brief View the page as `T`.
 *
 * Rust's `get_mut<T>` is generic; C returns a `void*` and the caller casts to
 * the desired type.
 */
void* phys_page_num_get_mut(PhysPageNum page_num);

/**
 * @brief A simple range structure for virtual page number.
 *
 * Rust equivalent: `SimpleRange<VirtPageNum>`, i.e. `VPNRange`.
 * Half-open interval `[left, right)`.
 */
typedef struct {
    VirtPageNum left;
    VirtPageNum right;
} VPNRange;

/**
 * @brief Iterator for @ref VPNRange .
 */
typedef struct {
    VirtPageNum current;
    VirtPageNum end;
} VPNRangeIterator;

/**
 * @brief Create a new range `[start, end)`.
 *
 * Panics if `start > end`.
 */
VPNRange vpn_range_new(VirtPageNum start, VirtPageNum end);

/**
 * @brief Return the inclusive start of the range.
 */
VirtPageNum vpn_range_get_start(VPNRange range);

/**
 * @brief Return the exclusive end of the range.
 */
VirtPageNum vpn_range_get_end(VPNRange range);

/**
 * @brief `IntoIterator::into_iter` for @ref VPNRange .
 */
VPNRangeIterator vpn_range_into_iter(VPNRange range);

/**
 * @brief Create a new iterator over `[left, right)`.
 */
VPNRangeIterator vpn_range_iterator_new(VirtPageNum left, VirtPageNum right);

/**
 * @brief `Iterator::next` for @ref VPNRangeIterator .
 *
 * @param[in,out] iter The iterator.
 * @param[out]    out  Receives the next VPN when the return value is `true`.
 * @return `true` if a value was produced, `false` when the range is exhausted.
 */
bool vpn_range_iterator_next(VPNRangeIterator* iter, VirtPageNum* out);

#endif /* __MM_ADDRESS_H_ */
