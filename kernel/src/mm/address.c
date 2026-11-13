/**
 * @file address.c
 * @brief Implementation of physical address and virtual address and page number.
 */

#include "address.h"
#include "../panic.h"

static const usize PA_WIDTH_SV39 = 56;
static const usize VA_WIDTH_SV39 = 39;
static const usize PPN_WIDTH_SV39 = PA_WIDTH_SV39 - PAGE_SIZE_BITS;
static const usize VPN_WIDTH_SV39 = VA_WIDTH_SV39 - PAGE_SIZE_BITS;

PhysAddr phys_addr_from_usize(usize value) {
    PhysAddr addr;
    addr.value = value & ((1ul << PA_WIDTH_SV39) - 1);
    return addr;
}

VirtAddr virt_addr_from_usize(usize value) {
    VirtAddr addr;
    addr.value = value & ((1ul << VA_WIDTH_SV39) - 1);
    return addr;
}

PhysPageNum phys_pnum_from_usize(usize value) {
    PhysPageNum ppn;
    ppn.value = value & ((1ul << PPN_WIDTH_SV39) - 1);
    return ppn;
}

VirtPageNum virt_pnum_from_usize(usize value) {
    VirtPageNum vpn;
    vpn.value = value & ((1ul << VPN_WIDTH_SV39) - 1);
    return vpn;
}

usize phys_addr_to_usize(PhysAddr addr) {
    return addr.value;
}

usize virt_addr_to_usize(VirtAddr addr) {
    if (addr.value >= (1ul << (VA_WIDTH_SV39 - 1))) {
        return addr.value | (~((1ul << VA_WIDTH_SV39) - 1));
    } else {
        return addr.value;
    }
}

usize phys_page_num_to_usize(PhysPageNum page_num) {
    return page_num.value;
}

usize virt_page_num_to_usize(VirtPageNum page_num) {
    return page_num.value;
}

VirtPageNum virt_addr_floor(VirtAddr addr) {
    VirtPageNum vpn;
    vpn.value = addr.value / PAGE_SIZE;
    return vpn;
}

VirtPageNum virt_addr_ceil(VirtAddr addr) {
    VirtPageNum vpn;
    if (addr.value == 0) {
        vpn.value = 0;
    } else {
        vpn.value = (addr.value - 1 + PAGE_SIZE) / PAGE_SIZE;
    }
    return vpn;
}

usize virt_addr_page_offset(VirtAddr addr) {
    return addr.value & (PAGE_SIZE - 1);
}

bool virt_addr_aligned(VirtAddr addr) {
    return virt_addr_page_offset(addr) == 0;
}

PhysPageNum phys_addr_floor(PhysAddr addr) {
    PhysPageNum ppn;
    ppn.value = addr.value / PAGE_SIZE;
    return ppn;
}

PhysPageNum phys_addr_ceil(PhysAddr addr) {
    PhysPageNum ppn;
    if (addr.value == 0) {
        ppn.value = 0;
    } else {
        ppn.value = (addr.value - 1 + PAGE_SIZE) / PAGE_SIZE;
    }
    return ppn;
}

usize phys_addr_page_offset(PhysAddr addr) {
    return addr.value & (PAGE_SIZE - 1);
}

bool phys_addr_aligned(PhysAddr addr) {
    return phys_addr_page_offset(addr) == 0;
}

VirtPageNum virt_page_num_from_virt_addr(VirtAddr addr) {
    assert_panic(virt_addr_aligned(addr));
    return virt_addr_floor(addr);
}

VirtAddr virt_addr_from_virt_page_num(VirtPageNum page_num) {
    VirtAddr addr;
    addr.value = page_num.value << PAGE_SIZE_BITS;
    return addr;
}

PhysPageNum phys_page_num_from_phys_addr(PhysAddr addr) {
    assert_panic(phys_addr_aligned(addr));
    return phys_addr_floor(addr);
}

PhysAddr phys_addr_from_phys_page_num(PhysPageNum page_num) {
    PhysAddr addr;
    addr.value = page_num.value << PAGE_SIZE_BITS;
    return addr;
}

void virt_page_num_indices(VirtPageNum page_num, usize out[3]) {
    usize vpn = page_num.value;
    for (int i = 2; i >= 0; i--) {
        out[i] = vpn & 511;
        vpn >>= 9;
    }
}

void virt_page_num_step(VirtPageNum* page_num) {
    page_num->value++;
}

struct PageTableEntry* phys_page_num_get_pte_array(PhysPageNum page_num) {
    PhysAddr pa = phys_addr_from_phys_page_num(page_num);
    return (struct PageTableEntry*)pa.value;
}

uint8_t* phys_page_num_get_bytes_array(PhysPageNum page_num) {
    PhysAddr pa = phys_addr_from_phys_page_num(page_num);
    return (uint8_t*)pa.value;
}

void* phys_page_num_get_mut(PhysPageNum page_num) {
    PhysAddr pa = phys_addr_from_phys_page_num(page_num);
    return (void*)pa.value;
}

VPNRange vpn_range_new(VirtPageNum start, VirtPageNum end) {
    if (!(start.value <= end.value)) {
        panic("start %lX > end %lX", (unsigned long)start.value, (unsigned long)end.value);
    }
    VPNRange range;
    range.left = start;
    range.right = end;
    return range;
}

VirtPageNum vpn_range_get_start(VPNRange range) {
    return range.left;
}

VirtPageNum vpn_range_get_end(VPNRange range) {
    return range.right;
}

VPNRangeIterator vpn_range_into_iter(VPNRange range) {
    return vpn_range_iterator_new(range.left, range.right);
}

VPNRangeIterator vpn_range_iterator_new(VirtPageNum left, VirtPageNum right) {
    VPNRangeIterator it;
    it.current = left;
    it.end = right;
    return it;
}

bool VPNRangeIterator_next(VPNRangeIterator* iter, VirtPageNum* out) {
    if (iter->current.value == iter->end.value) {
        return false;
    } else {
        *out = iter->current;
        virt_page_num_step(&iter->current);
        return true;
    }
}
