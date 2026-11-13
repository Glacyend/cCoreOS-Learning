/**
 * @file elf.c
 * @brief Minimal ELF64 reader.
 */

#include "elf.h"
#include "../panic.h"

static uint16_t read_u16_le(const uint8_t* ptr) {
    return (uint16_t)ptr[0] | ((uint16_t)ptr[1] << 8);
}

static uint32_t read_u32_le(const uint8_t* ptr) {
    return (uint32_t)ptr[0] | ((uint32_t)ptr[1] << 8) | ((uint32_t)ptr[2] << 16)
         | ((uint32_t)ptr[3] << 24);
}

static uint64_t read_u64_le(const uint8_t* ptr) {
    return (uint64_t)read_u32_le(ptr) | ((uint64_t)read_u32_le(ptr + 4) << 32);
}

bool elf_file_new(ElfFile* file, const uint8_t* data, usize len) {
    if (len < 64) {
        return false;
    }
    if (data[0] != 0x7F || data[1] != 'E' || data[2] != 'L' || data[3] != 'F') {
        return false;
    }
    if (data[4] != 2) {
        return false;  // not ELFCLASS64
    }
    if (data[5] != 1) {
        return false;  // not little-endian
    }
    file->input = data;
    file->entry_point = read_u64_le(data + 24);
    file->ph_offset = read_u64_le(data + 32);
    file->ph_entry_size = read_u16_le(data + 54);
    file->ph_count = read_u16_le(data + 56);
    return true;
}

void elf_program_header(const ElfFile* file, uint16_t idx, ElfProgramHeader* out) {
    if (idx >= file->ph_count) {
        panic(
            "elf_program_header: idx %u out of range (count = %u)",
            (unsigned)idx,
            (unsigned)file->ph_count
        );
    }
    const uint8_t* ptr = file->input + file->ph_offset + (usize)idx * file->ph_entry_size;
    out->type = read_u32_le(ptr);
    out->flags = read_u32_le(ptr + 4);
    out->offset = read_u64_le(ptr + 8);
    out->virtual_addr = read_u64_le(ptr + 16);
    out->file_size = read_u64_le(ptr + 32);
    out->mem_size = read_u64_le(ptr + 40);
}
