/**
 * @file page_table.h
 * @brief Implementation of `PageTableEntry` and `PageTable`.
 */

#ifndef __MM_PAGE_TABLE_H_
#define __MM_PAGE_TABLE_H_

#include "address.h"
#include "vec.h"

/**
 * @brief Page table entry flags.
 *
 * Rust equivalent: `PTEFlags`.
 */
typedef uint8_t PTEFlags;

#define PTEFlags_V (1u << 0)
#define PTEFlags_R (1u << 1)
#define PTEFlags_W (1u << 2)
#define PTEFlags_X (1u << 3)
#define PTEFlags_U (1u << 4)
#define PTEFlags_G (1u << 5)
#define PTEFlags_A (1u << 6)
#define PTEFlags_D (1u << 7)

/**
 * @brief Page table entry structure.
 *
 * Rust equivalent: `PageTableEntry`. `#[repr(C)]` maps to a plain C struct;
 * the layout is 8 bytes.
 */
typedef struct PageTableEntry {
    usize bits;
} PageTableEntry;

/**
 * @brief Page table structure.
 *
 * Rust equivalent: `PageTable`.
 */
typedef struct {
    PhysPageNum root_ppn;

    /** @brief `Vec<FrameTracker>`. */
    Vector frames;
} PageTable;

/**
 * @brief Construct a new PTE.
 *
 * Rust equivalent: `PageTableEntry::new`.
 */
PageTableEntry page_table_entry_new(PhysPageNum ppn, PTEFlags flags);

/**
 * @brief Construct an empty PTE.
 *
 * Rust equivalent: `PageTableEntry::empty`.
 */
PageTableEntry page_table_entry_empty();

/**
 * @brief Return the physical page number stored in the PTE.
 *
 * Rust equivalent: `PageTableEntry::ppn`.
 */
PhysPageNum page_table_entry_ppn(const PageTableEntry* pte);

/**
 * @brief Return the flags of the PTE.
 *
 * Rust equivalent: `PageTableEntry::flags`.
 */
PTEFlags page_table_entry_flags(const PageTableEntry* pte);

/**
 * @brief Return whether the PTE is valid (`V` set).
 *
 * Rust equivalent: `PageTableEntry::is_valid`.
 */
bool page_table_entry_is_valid(const PageTableEntry* pte);

/**
 * @brief Return whether the PTE is readable (`R` set).
 *
 * Rust equivalent: `PageTableEntry::readable`.
 */
bool page_table_entry_readable(const PageTableEntry* pte);

/**
 * @brief Return whether the PTE is writable (`W` set).
 *
 * Rust equivalent: `PageTableEntry::writable`.
 */
bool page_table_entry_writable(const PageTableEntry* pte);

/**
 * @brief Return whether the PTE is executable (`X` set).
 *
 * Rust equivalent: `PageTableEntry::executable`.
 */
bool page_table_entry_executable(const PageTableEntry* pte);

/**
 * @brief Allocate a new root page table.
 *
 * Rust equivalent: `PageTable::new`.
 */
void page_table_new(PageTable* table);

/**
 * @brief Construct a `PageTable` from a `satp` token.
 *
 * Temporarily used to get arguments from user space.
 *
 * Rust equivalent: `PageTable::from_token`.
 */
void page_table_from_token(PageTable* table, usize satp);

/**
 * @brief Map @p vpn to @p ppn with @p flags .
 *
 * Rust equivalent: `PageTable::map`.
 */
void page_table_map(PageTable* table, VirtPageNum vpn, PhysPageNum ppn, PTEFlags flags);

/**
 * @brief Unmap @p vpn .
 *
 * Rust equivalent: `PageTable::unmap`.
 */
void page_table_unmap(PageTable* table, VirtPageNum vpn);

/**
 * @brief Translate @p vpn to a PTE.
 *
 * @param      table Page table.
 * @param      vpn   Virtual page number.
 * @param[out] out   Receives the PTE when the return value is `true`.
 * @return `true` if a valid PTE was found, `false` otherwise.
 */
bool page_table_translate(PageTable* table, VirtPageNum vpn, PageTableEntry* out);

/**
 * @brief Return the `satp` token of this page table.
 *
 * Rust equivalent: `PageTable::token`.
 */
usize page_table_token(const PageTable* table);

/**
 * @brief Translate a pointer to a mutable byte buffer through the page table.
 *
 * Rust equivalent: `translated_byte_buffer`. On success, `*out` is initialized
 * to a @ref Vector of @ref ByteSlice . The caller must call `vec_free(out)`
 * after use.
 */
void translated_byte_buffer(Vector* out, usize token, const uint8_t* ptr, usize len);

#endif
