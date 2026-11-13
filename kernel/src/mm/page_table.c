/**
 * @file page_table.c
 * @brief Implementation of `PageTableEntry` and `PageTable`.
 */

#include "page_table.h"
#include "../panic.h"
#include "frame_allocator.h"
#include "slice.h"

#include <stddef.h>

PageTableEntry page_table_entry_new(PhysPageNum ppn, PTEFlags flags) {
    PageTableEntry pte;
    pte.bits = ppn.value << 10 | (usize)flags;
    return pte;
}

PageTableEntry page_table_entry_empty() {
    PageTableEntry pte;
    pte.bits = 0;
    return pte;
}

PhysPageNum page_table_entry_ppn(const PageTableEntry* pte) {
    return phys_page_num_from_usize((pte->bits >> 10) & ((1ul << 44) - 1));
}

PTEFlags page_table_entry_flags(const PageTableEntry* pte) {
    return (PTEFlags)pte->bits;
}

bool page_table_entry_is_valid(const PageTableEntry* pte) {
    return (page_table_entry_flags(pte) & PTEFlags_V) != 0;
}

bool page_table_entry_is_readable(const PageTableEntry* pte) {
    return (page_table_entry_flags(pte) & PTEFlags_R) != 0;
}

bool page_table_entry_is_writable(const PageTableEntry* pte) {
    return (page_table_entry_flags(pte) & PTEFlags_W) != 0;
}

bool page_table_entry_is_executable(const PageTableEntry* pte) {
    return (page_table_entry_flags(pte) & PTEFlags_X) != 0;
}

void page_table_new(PageTable* table) {
    FrameTracker frame;
    if (!frame_alloc(&frame)) {
        panic("page_table_new: out of memory");
    }
    table->root_ppn = frame.ppn;
    vec_init(&table->frames, sizeof(FrameTracker));
    if (!vec_push(&table->frames, &frame)) {
        panic("page_table_new: out of memory");
    }
}

void page_table_from_token(PageTable* table, usize satp) {
    table->root_ppn = phys_page_num_from_usize(satp & ((1ul << 44) - 1));
    vec_init(&table->frames, sizeof(FrameTracker));
}

/**
 * @brief Walk the three-level page table and create missing middle pages.
 *
 * Rust equivalent: `PageTable::find_pte_create`.
 */
static PageTableEntry* page_table_find_pte_create(PageTable* table, VirtPageNum vpn) {
    usize indices[3];
    virt_page_num_indices(vpn, indices);
    PhysPageNum ppn = table->root_ppn;
    PageTableEntry* result = NULL;
    for (usize i = 0; i < 3; i++) {
        PageTableEntry* pte = &phys_page_num_get_pte_array(ppn)[indices[i]];
        if (i == 2) {
            result = pte;
            break;
        }
        if (!page_table_entry_is_valid(pte)) {
            FrameTracker frame;
            if (!frame_alloc(&frame)) {
                panic("page_table_find_pte_create: out of memory");
            }
            *pte = page_table_entry_new(frame.ppn, PTEFlags_V);
            if (!vec_push(&table->frames, &frame)) {
                panic("page_table_find_pte_create: out of memory");
            }
        }
        ppn = page_table_entry_ppn(pte);
    }
    return result;
}

/**
 * @brief Walk the three-level page table without creating anything.
 *
 * Rust equivalent: `PageTable::find_pte`.
 */
static PageTableEntry* page_table_find_pte(PageTable* table, VirtPageNum vpn) {
    usize indices[3];
    virt_page_num_indices(vpn, indices);
    PhysPageNum ppn = table->root_ppn;
    PageTableEntry* result = NULL;
    for (usize i = 0; i < 3; i++) {
        PageTableEntry* pte = &phys_page_num_get_pte_array(ppn)[indices[i]];
        if (i == 2) {
            result = pte;
            break;
        }
        if (!page_table_entry_is_valid(pte)) {
            return NULL;
        }
        ppn = page_table_entry_ppn(pte);
    }
    return result;
}

void page_table_map(PageTable* table, VirtPageNum vpn, PhysPageNum ppn, PTEFlags flags) {
    PageTableEntry* pte = page_table_find_pte_create(table, vpn);
    if (pte == NULL) {
        panic("page_table_map: page_table_find_pte_create returned NULL");
    }
    if (page_table_entry_is_valid(pte)) {
        panic("vpn %lX is mapped before mapping", (unsigned long)vpn.value);
    }
    *pte = page_table_entry_new(ppn, flags | PTEFlags_V);
}

void page_table_unmap(PageTable* table, VirtPageNum vpn) {
    PageTableEntry* pte = page_table_find_pte(table, vpn);
    if (pte == NULL) {
        panic("vpn %lX is invalid before unmapping", (unsigned long)vpn.value);
    }
    if (!page_table_entry_is_valid(pte)) {
        panic("vpn %lX is invalid before unmapping", (unsigned long)vpn.value);
    }
    *pte = page_table_entry_empty();
}

bool page_table_translate(PageTable* table, VirtPageNum vpn, PageTableEntry* out) {
    PageTableEntry* pte = page_table_find_pte(table, vpn);
    if (pte == NULL) {
        return false;
    }
    *out = *pte;
    return true;
}

usize page_table_token(const PageTable* table) {
    return (8ul << 60) | table->root_ppn.value;
}

void translated_byte_buffer(Vector* out, usize token, const uint8_t* ptr, usize len) {
    PageTable page_table;
    page_table_from_token(&page_table, token);
    usize start = (usize)ptr;
    usize end = start + len;
    vec_init(out, sizeof(ByteSlice));
    while (start < end) {
        VirtAddr start_va = virt_addr_from_usize(start);
        VirtPageNum vpn = virt_addr_floor(start_va);
        PageTableEntry pte;
        if (!page_table_translate(&page_table, vpn, &pte)) {
            panic("translated_byte_buffer: unmapped page at 0x%lX", (unsigned long)start);
        }
        PhysPageNum ppn = page_table_entry_ppn(&pte);
        virt_page_num_step(&vpn);
        VirtAddr end_va = virt_addr_from_virt_page_num(vpn);
        VirtAddr end_va_bound = virt_addr_from_usize(end);
        if (end_va.value > end_va_bound.value) {
            end_va = end_va_bound;
        }
        ByteSlice slice;
        slice.data = phys_page_num_get_bytes_array(ppn) + virt_addr_page_offset(start_va);
        if (virt_addr_aligned(end_va)) {
            slice.len = PAGE_SIZE - virt_addr_page_offset(start_va);
        } else {
            slice.len = virt_addr_page_offset(end_va) - virt_addr_page_offset(start_va);
        }
        if (!vec_push(out, &slice)) {
            panic("translated_byte_buffer: out of memory");
        }
        start = virt_addr_to_usize(end_va);
    }
}
