/**
 * @file memory_set.c
 * @brief Implementation of `MapArea` and `MemorySet`.
 */

#include "memory_set.h"
#include "../boards/qemu.h"
#include "../logging.h"
#include "../panic.h"
#include "elf.h"
#include "frame_allocator.h"
#include "map.h"

#include <stddef.h>

/**
 * @brief Map type for `MemorySet`: `Identical` or `Framed`.
 *
 * Rust equivalent: `MapType`.
 */
typedef enum {
    MAP_TYPE_IDENTICAL,
    MAP_TYPE_FRAMED,
} MapType;

/**
 * @brief Map area structure, controls a contiguous piece of virtual memory.
 *
 * Rust equivalent: `MapArea`.
 */
typedef struct {
    VPNRange vpn_range;
    VpnFrameMap data_frames;
    MapType map_type;
    MapPermission map_perm;
} MapArea;

extern char stext[];
extern char etext[];
extern char srodata[];
extern char erodata[];
extern char sdata[];
extern char edata[];
extern char sbss_with_stack[];
extern char ebss[];
extern char ekernel[];
extern char strampoline[];

/**
 * @brief The global kernel memory set.
 *
 * Rust equivalent: `KERNEL_SPACE: Arc<UPSafeCell<MemorySet>>`, lazily
 * initialized on first access. In C the initialization is explicit via @ref
 * init_kernel_space .
 */
static MemorySet global_kernel_space;
static bool global_kernel_space_initialized = false;

MemorySet* kernel_space() {
    return &global_kernel_space;
}

void init_kernel_space() {
    if (!global_kernel_space_initialized) {
        memory_set_new_kernel(&global_kernel_space);
        global_kernel_space_initialized = true;
    }
}

/**
 * @brief Internal constructor for `MapArea`.
 *
 * Rust equivalent: `MapArea::new`.
 */
static void map_area_new(
    MapArea* area, VirtAddr start_va, VirtAddr end_va, MapType map_type, MapPermission map_perm
) {
    VirtPageNum start_vpn = virt_addr_floor(start_va);
    VirtPageNum end_vpn = virt_addr_ceil(end_va);
    area->vpn_range = vpn_range_new(start_vpn, end_vpn);
    vpn_frame_map_init(&area->data_frames);
    area->map_type = map_type;
    area->map_perm = map_perm;
}

/**
 * @brief Map one VPN according to @p area->map_type .
 *
 * Rust equivalent: `MapArea::map_one`.
 */
static void map_area_map_one(MapArea* area, PageTable* page_table, VirtPageNum vpn) {
    PhysPageNum ppn;
    if (area->map_type == MAP_TYPE_IDENTICAL) {
        ppn = phys_page_num_from_usize(vpn.value);
    } else {
        FrameTracker frame;
        if (!frame_alloc(&frame)) {
            panic("map_area_map_one: out of memory");
        }
        ppn = frame.ppn;
        vpn_frame_map_insert(&area->data_frames, vpn, frame);
    }
    PTEFlags pte_flags = (PTEFlags)area->map_perm;
    page_table_map(page_table, vpn, ppn, pte_flags);
}

/**
 * @brief Unmap one VPN and drop its data frame.
 *
 * Rust equivalent: `MapArea::unmap_one`.
 */
static void map_area_unmap_one(MapArea* area, PageTable* page_table, VirtPageNum vpn) {
    if (area->map_type == MAP_TYPE_FRAMED) {
        FrameTracker removed;
        if (vpn_frame_map_remove(&area->data_frames, vpn, &removed)) {
            // drop the FrameTracker, matching `BTreeMap::remove`.
            frame_dealloc(removed.ppn);
        }
    }
    page_table_unmap(page_table, vpn);
}

/**
 * @brief Map every VPN in @p area->vpn_range .
 *
 * Rust equivalent: `MapArea::map`.
 */
static void map_area_map(MapArea* area, PageTable* page_table) {
    VPNRangeIterator it = vpn_range_into_iter(area->vpn_range);
    VirtPageNum vpn;
    while (vpn_range_iterator_next(&it, &vpn)) {
        map_area_map_one(area, page_table, vpn);
    }
}

/**
 * @brief Unmap every VPN in @p area->vpn_range .
 *
 * Rust equivalent: `MapArea::unmap`.
 */
[[maybe_unused]] static void map_area_unmap(MapArea* area, PageTable* page_table) {
    VPNRangeIterator it = vpn_range_into_iter(area->vpn_range);
    VirtPageNum vpn;
    while (vpn_range_iterator_next(&it, &vpn)) {
        map_area_unmap_one(area, page_table, vpn);
    }
}

/**
 * @brief Shrink @p area to end at @p new_end .
 *
 * Rust equivalent: `MapArea::shrink_to`.
 */
static void map_area_shrink_to(MapArea* area, PageTable* page_table, VirtPageNum new_end) {
    VPNRange tail = vpn_range_new(new_end, area->vpn_range.right);
    VPNRangeIterator it = vpn_range_into_iter(tail);
    VirtPageNum vpn;
    while (vpn_range_iterator_next(&it, &vpn)) {
        map_area_unmap_one(area, page_table, vpn);
    }
    area->vpn_range = vpn_range_new(area->vpn_range.left, new_end);
}

/**
 * @brief Grow @p area to end at @p new_end .
 *
 * Rust equivalent: `MapArea::append_to`.
 */
static void map_area_append_to(MapArea* area, PageTable* page_table, VirtPageNum new_end) {
    VPNRange tail = vpn_range_new(area->vpn_range.right, new_end);
    VPNRangeIterator it = vpn_range_into_iter(tail);
    VirtPageNum vpn;
    while (vpn_range_iterator_next(&it, &vpn)) {
        map_area_map_one(area, page_table, vpn);
    }
    area->vpn_range = vpn_range_new(area->vpn_range.left, new_end);
}

/**
 * @brief Copy ELF bytes into the frames already mapped by @p area .
 *
 * Rust equivalent: `MapArea::copy_data`.
 *
 * @param data Start-aligned but maybe with shorter length.
 *
 * @note Assume that all frames were cleared before.
 */
static void map_area_copy_data(
    MapArea* area, PageTable* page_table, const uint8_t* data, usize len
) {
    if (area->map_type != MAP_TYPE_FRAMED) {
        panic("map_area_copy_data: map_type is not Framed");
    }
    usize start = 0;
    VirtPageNum current_vpn = area->vpn_range.left;
    while (true) {
        usize chunk = PAGE_SIZE;
        if (len - start < chunk) {
            chunk = len - start;
        }
        PageTableEntry pte;
        if (!page_table_translate(page_table, current_vpn, &pte)) {
            panic("map_area_copy_data: unmapped vpn 0x%lX", (unsigned long)current_vpn.value);
        }
        PhysPageNum ppn = page_table_entry_ppn(&pte);
        uint8_t* dst = phys_page_num_get_bytes_array(ppn);
        for (usize i = 0; i < chunk; i++) {
            dst[i] = data[start + i];
        }
        start += PAGE_SIZE;
        if (start >= len) {
            break;
        }
        virt_page_num_step(&current_vpn);
    }
}

/**
 * @brief Map @p map_area and copy @p data into it, then store it.
 *
 * Rust equivalent: `MemorySet::push`.
 */
static void memory_set_push(
    MemorySet* set, MapArea* map_area, const uint8_t* data, usize data_len
) {
    map_area_map(map_area, &set->page_table);
    if (data != NULL) {
        map_area_copy_data(map_area, &set->page_table, data, data_len);
    }
    if (!vec_push(&set->areas, map_area)) {
        panic("memory_set_push: out of memory");
    }
}

/**
 * @brief Map the trampoline page.
 *
 * Rust equivalent: `MemorySet::map_trampoline`.
 *
 * @note Mention that trampoline is not collected by areas.
 */
static void memory_set_map_trampoline(MemorySet* set) {
    VirtPageNum vpn = virt_page_num_from_virt_addr(virt_addr_from_usize(TRAMPOLINE));
    PhysPageNum ppn = phys_page_num_from_phys_addr(phys_addr_from_usize((usize)strampoline));
    page_table_map(&set->page_table, vpn, ppn, PTEFlags_R | PTEFlags_X);
}

void memory_set_new_bare(MemorySet* set) {
    page_table_new(&set->page_table);
    vec_init(&set->areas, sizeof(MapArea));
}

usize memory_set_token(const MemorySet* set) {
    return page_table_token(&set->page_table);
}

void memory_set_insert_framed_area(
    MemorySet* set, VirtAddr start_va, VirtAddr end_va, MapPermission permission
) {
    MapArea map_area;
    map_area_new(&map_area, start_va, end_va, MAP_TYPE_FRAMED, permission);
    memory_set_push(set, &map_area, NULL, 0);
}

void memory_set_new_kernel(MemorySet* set) {
    memory_set_new_bare(set);

    // map trampoline
    memory_set_map_trampoline(set);

    // map kernel sections
    trace(".text [0x%lX, 0x%lX)", (unsigned long)(usize)stext, (unsigned long)(usize)etext);
    trace(".rodata [0x%lX, 0x%lX)", (unsigned long)(usize)srodata, (unsigned long)(usize)erodata);
    trace(".data [0x%lX, 0x%lX)", (unsigned long)(usize)sdata, (unsigned long)(usize)edata);
    trace(".bss [0x%lX, 0x%lX)", (unsigned long)(usize)sbss_with_stack, (unsigned long)(usize)ebss);

    trace("Mapping .text section.");
    MapArea text_area;
    map_area_new(
        &text_area,
        virt_addr_from_usize((usize)stext),
        virt_addr_from_usize((usize)etext),
        MAP_TYPE_IDENTICAL,
        MapPermission_R | MapPermission_X
    );
    memory_set_push(set, &text_area, NULL, 0);

    trace("Mapping .rodata section.");
    MapArea rodata_area;
    map_area_new(
        &rodata_area,
        virt_addr_from_usize((usize)srodata),
        virt_addr_from_usize((usize)erodata),
        MAP_TYPE_IDENTICAL,
        MapPermission_R
    );
    memory_set_push(set, &rodata_area, NULL, 0);

    trace("Mapping .data section.");
    MapArea data_area;
    map_area_new(
        &data_area,
        virt_addr_from_usize((usize)sdata),
        virt_addr_from_usize((usize)edata),
        MAP_TYPE_IDENTICAL,
        MapPermission_R | MapPermission_W
    );
    memory_set_push(set, &data_area, NULL, 0);

    trace("Mapping .bss section.");
    MapArea bss_area;
    map_area_new(
        &bss_area,
        virt_addr_from_usize((usize)sbss_with_stack),
        virt_addr_from_usize((usize)ebss),
        MAP_TYPE_IDENTICAL,
        MapPermission_R | MapPermission_W
    );
    memory_set_push(set, &bss_area, NULL, 0);

    trace("Mapping physical memory.");
    MapArea phys_area;
    map_area_new(
        &phys_area,
        virt_addr_from_usize((usize)ekernel),
        virt_addr_from_usize(MEMORY_END),
        MAP_TYPE_IDENTICAL,
        MapPermission_R | MapPermission_W
    );
    memory_set_push(set, &phys_area, NULL, 0);
}

void memory_set_from_elf(
    MemorySet* set, const uint8_t* elf_data, usize elf_len, usize* user_sp, usize* entry
) {
    memory_set_new_bare(set);

    // map trampoline
    memory_set_map_trampoline(set);

    // map program header of elf, with U flag
    ElfFile elf;
    if (!elf_file_new(&elf, elf_data, elf_len)) {
        panic("memory_set_from_elf: invalid elf");
    }
    VirtPageNum max_end_vpn = virt_page_num_from_usize(0);
    for (usize i = 0; i < (usize)elf.ph_count; i++) {
        ElfProgramHeader ph;
        elf_program_header(&elf, (uint16_t)i, &ph);
        if (ph.type == ELF_PT_LOAD) {
            VirtAddr start_va = virt_addr_from_usize((usize)ph.virtual_addr);
            VirtAddr end_va = virt_addr_from_usize((usize)(ph.virtual_addr + ph.mem_size));
            MapPermission map_perm = MapPermission_U;
            if ((ph.flags & ELF_PF_R) != 0) {
                map_perm |= MapPermission_R;
            }
            if ((ph.flags & ELF_PF_W) != 0) {
                map_perm |= MapPermission_W;
            }
            if ((ph.flags & ELF_PF_X) != 0) {
                map_perm |= MapPermission_X;
            }
            MapArea map_area;
            map_area_new(&map_area, start_va, end_va, MAP_TYPE_FRAMED, map_perm);
            max_end_vpn = map_area.vpn_range.right;
            memory_set_push(set, &map_area, elf_data + (usize)ph.offset, (usize)ph.file_size);
        }
    }

    // map user stack with U flags
    VirtAddr max_end_va = virt_addr_from_virt_page_num(max_end_vpn);
    usize user_stack_bottom = virt_addr_to_usize(max_end_va);

    // guard page
    user_stack_bottom += PAGE_SIZE;

    usize user_stack_top = user_stack_bottom + USER_STACK_SIZE;
    MapArea stack_area;
    map_area_new(
        &stack_area,
        virt_addr_from_usize(user_stack_bottom),
        virt_addr_from_usize(user_stack_top),
        MAP_TYPE_FRAMED,
        MapPermission_R | MapPermission_W | MapPermission_U
    );
    memory_set_push(set, &stack_area, NULL, 0);

    // used in sbrk
    MapArea heap_area;
    map_area_new(
        &heap_area,
        virt_addr_from_usize(user_stack_top),
        virt_addr_from_usize(user_stack_top),
        MAP_TYPE_FRAMED,
        MapPermission_R | MapPermission_W | MapPermission_U
    );
    memory_set_push(set, &heap_area, NULL, 0);

    // map `TrapContext`
    MapArea trap_cx_area;
    map_area_new(
        &trap_cx_area,
        virt_addr_from_usize(TRAP_CONTEXT),
        virt_addr_from_usize(TRAMPOLINE),
        MAP_TYPE_FRAMED,
        MapPermission_R | MapPermission_W
    );
    memory_set_push(set, &trap_cx_area, NULL, 0);

    *user_sp = user_stack_top;
    *entry = (usize)elf.entry_point;
}

void memory_set_activate(MemorySet* set) {
    usize satp = page_table_token(&set->page_table);
    __asm__ volatile("csrw satp, %0" : : "r"(satp));
    __asm__ volatile("sfence.vma");
}

bool memory_set_translate(MemorySet* set, VirtPageNum vpn, PageTableEntry* out) {
    return page_table_translate(&set->page_table, vpn, out);
}

/**
 * @brief Return the index of the area whose start VPN equals @p start .
 *
 * @return `-1` if no such area exists.
 */
static isize memory_set_find_area(MemorySet* set, VirtAddr start) {
    VirtPageNum start_vpn = virt_addr_floor(start);
    usize len = vec_len(&set->areas);
    for (usize i = 0; i < len; i++) {
        MapArea* area = (MapArea*)vec_get_mut(&set->areas, i);
        if (area->vpn_range.left.value == start_vpn.value) {
            return (isize)i;
        }
    }
    return -1;
}

bool memory_set_shrink_to(MemorySet* set, VirtAddr start, VirtAddr new_end) {
    isize idx = memory_set_find_area(set, start);
    if (idx < 0) {
        return false;
    }
    MapArea* area = (MapArea*)vec_get_mut(&set->areas, (usize)idx);
    map_area_shrink_to(area, &set->page_table, virt_addr_ceil(new_end));
    return true;
}

bool memory_set_append_to(MemorySet* set, VirtAddr start, VirtAddr new_end) {
    isize idx = memory_set_find_area(set, start);
    if (idx < 0) {
        return false;
    }
    MapArea* area = (MapArea*)vec_get_mut(&set->areas, (usize)idx);
    map_area_append_to(area, &set->page_table, virt_addr_ceil(new_end));
    return true;
}
