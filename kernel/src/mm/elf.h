/**
 * @file elf.h
 * @brief Minimal ELF64 reader.
 *
 * Rust uses the `xmas-elf` crate. The kernel only needs the ELF header and the
 * program header table, so a hand-written parser is enough. Only little-endian
 * ELF64 is supported, matching the QEMU virt toolchain output.
 */

#ifndef __MM_ELF_H_
#define __MM_ELF_H_

#include "../config.h"

/**
 * @brief `p_type` value for a loadable segment.
 */
#define ELF_PT_LOAD 1u

/**
 * @brief `p_flags` bits.
 */
#define ELF_PF_X (1u << 0)
#define ELF_PF_W (1u << 1)
#define ELF_PF_R (1u << 2)

/**
 * @brief A parsed program header.
 *
 * Rust equivalent: `xmas_elf::program::ProgramHeader`.
 */
typedef struct {
    /** @brief `p_type`. */
    uint32_t type;

    /** @brief `p_flags`. */
    uint32_t flags;

    /** @brief `p_offset`. */
    uint64_t offset;

    /** @brief `p_vaddr`. */
    uint64_t virtual_addr;

    /** @brief `p_filesz`. */
    uint64_t file_size;

    /** @brief `p_memsz`. */
    uint64_t mem_size;
} ElfProgramHeader;

/**
 * @brief A parsed ELF file.
 *
 * Rust equivalent: `xmas_elf::ElfFile`.
 */
typedef struct {
    /** @brief Pointer to the ELF bytes. */
    const uint8_t* input;

    /** @brief ELF entry point. */
    uint64_t entry_point;

    /** @brief Program header table offset. */
    uint64_t ph_offset;

    /** @brief Number of program headers. */
    uint16_t ph_count;
    
    /** @brief Program header entry size. */
    uint16_t ph_entry_size;
} ElfFile;

/**
 * @brief Parse the ELF header.
 *
 * Rust equivalent: `ElfFile::new(...).unwrap()` followed by the magic
 * assertion. Returns `false` if the input is not a little-endian ELF64 file.
 */
bool elf_file_new(ElfFile* file, const uint8_t* data, usize len);

/**
 * @brief Read program header @p idx .
 *
 * Rust equivalent: `ElfFile::program_header(idx).unwrap()`. Caller must ensure
 * `idx < file->ph_count`.
 */
void elf_program_header(const ElfFile* file, uint16_t idx, ElfProgramHeader* out);

#endif /* __MM_ELF_H_ */
