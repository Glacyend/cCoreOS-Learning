/**
 * @file memory_set.h
 * @brief Implementation of `MapArea` and `MemorySet`.
 */

#ifndef __MM_MEMORY_SET_H_
#define __MM_MEMORY_SET_H_

#include "address.h"
#include "page_table.h"
#include "vec.h"

/**
 * @brief Map permission corresponding to that in pte: `R W X U`.
 *
 * Rust equivalent: `MapPermission`. The bit values match `PTEFlags`.
 */
typedef uint8_t MapPermission;

#define MapPermission_R (1u << 1)
#define MapPermission_W (1u << 2)
#define MapPermission_X (1u << 3)
#define MapPermission_U (1u << 4)

/**
 * @brief Memory set structure, controls virtual-memory space.
 *
 * Rust equivalent: `MemorySet`.
 */
typedef struct {
    PageTable page_table;

    /** @brief `Vec<MapArea>`. */
    Vector areas;
} MemorySet;

/**
 * @brief Return the global kernel memory set.
 *
 * Rust equivalent: `&KERNEL_SPACE`.
 */
MemorySet* kernel_space();

/**
 * @brief Initialize the global kernel memory set.
 *
 * Must be called once before @ref kernel_space is used.
 *
 * Rust equivalent: `KERNEL_SPACE` being lazily initialized on first access.
 */
void init_kernel_space();

/**
 * @brief Construct an empty memory set with a fresh root page table.
 *
 * Rust equivalent: `MemorySet::new_bare`.
 */
void memory_set_new_bare(MemorySet* set);

/**
 * @brief Return the `satp` token of this memory set.
 *
 * Rust equivalent: `MemorySet::token`.
 */
usize memory_set_token(const MemorySet* set);

/**
 * @brief Map `[start_va, end_va)` as a `Framed` area.
 *
 * Rust equivalent: `MemorySet::insert_framed_area`.
 *
 * @note Assume that no conflicts.
 */
void memory_set_insert_framed_area(
    MemorySet* set, VirtAddr start_va, VirtAddr end_va, MapPermission permission
);

/**
 * @brief Construct the kernel memory set.
 *
 * Maps the trampoline, `.text`, `.rodata`, `.data`, `.bss`, and the remaining
 * physical memory.
 *
 * Rust equivalent: `MemorySet::new_kernel`.
 *
 * @note Without kernel stacks; those are inserted by `TaskControlBlock::new`.
 */
void memory_set_new_kernel(MemorySet* set);

/**
 * @brief Construct a user memory set from an ELF image.
 *
 * Maps the trampoline, every `PT_LOAD` program header, the user stack (with a
 * guard page before it), and `TRAP_CONTEXT`.
 *
 * Rust equivalent: `MemorySet::from_elf`.
 *
 * @param set      Memory set to initialize.
 * @param elf_data ELF bytes.
 * @param elf_len  Length of @p elf_data .
 * @param user_sp  Receives the user stack top.
 * @param entry    Receives the entry point.
 */
void memory_set_from_elf(
    MemorySet* set, const uint8_t* elf_data, usize elf_len, usize* user_sp, usize* entry
);

/**
 * @brief Activate this memory set by writing `satp` and flushing the TLB.
 *
 * Rust equivalent: `MemorySet::activate`.
 */
void memory_set_activate(MemorySet* set);

/**
 * @brief Translate `vpn` to a PTE.
 *
 * Rust equivalent: `MemorySet::translate`.
 *
 * @return `true` if a valid PTE was found, `false` otherwise.
 */
bool memory_set_translate(MemorySet* set, VirtPageNum vpn, PageTableEntry* out);

/**
 * @brief Shrink the area that starts at @p start to end at @p new_end .
 *
 * Rust equivalent: `MemorySet::shrink_to`.
 *
 * @return `true` if a matching area was found.
 */
bool memory_set_shrink_to(MemorySet* set, VirtAddr start, VirtAddr new_end);

/**
 * @brief Grow the area that starts at @p start to end at @p new_end .
 *
 * Rust equivalent: `MemorySet::append_to`.
 *
 * @return `true` if a matching area was found.
 */
bool memory_set_append_to(MemorySet* set, VirtAddr start, VirtAddr new_end);

#endif /* __MM_MEMORY_SET_H_ */
