/**
 * @file device_tree.c
 * @brief Minimal FDT parser.
 *
 * FDT layout (all multi-byte fields are big-endian):
 *
 *   +--------------------+
 *   | header (40 bytes)  |
 *   +--------------------+
 *   | memory reservation |
 *   +--------------------+
 *   | structure block    |  <-- nodes/properties
 *   +--------------------+
 *   | strings block      |  <-- property name
 *   +--------------------+
 *
 * The structure block is a sequence of 32-bit tokens:
 *   FDT_BEGIN_NODE (0x1) : node name (NUL-terminated, 4-byte aligned)
 *   FDT_END_NODE   (0x2)
 *   FDT_PROP       (0x3) : len, nameoff, then `len` bytes of value
 *   FDT_NOP        (0x4)
 *   FDT_END        (0x9)
 *
 * We only need to find a property called `timebase-frequency`, which in QEMU
 * virt lives under `/cpus`.
 */

#include "device_tree.h"
#include "logging.h"
#include "panic.h"

// FDT header magic.
#define FDT_MAGIC 0xD00DFEEDul

// Structure-block tokens.
#define FDT_BEGIN_NODE 0x1ul
#define FDT_END_NODE 0x2ul
#define FDT_PROP 0x3ul
#define FDT_NOP 0x4ul
#define FDT_END 0x9ul

/** @brief Parsed header fields we care about. */
typedef struct {
    usize off_struct; /**< Offset from DTB base to structure block. */
    usize off_strings; /**< Offset from DTB base to strings block. */
} FdtHeader;

/** @brief Clock frequency in Hz; 0 until @ref device_tree_init . */
static usize global_clock_freq = 0;

/**
 * @brief Read a big-endian 32-bit value.
 *
 * @param p Pointer to the first byte of the value.
 * @return The value.
 */
static inline unsigned int read_be32(const unsigned char* p) {
    return ((unsigned int)p[0] << 24) | ((unsigned int)p[1] << 16) | ((unsigned int)p[2] << 8)
         | (unsigned int)p[3];
}

/**
 * @brief Parse the FDT header.
 *
 * @param dtb_pa Physical address of the DTB.
 * @param out    Receives the parsed offsets.
 * @return 0 on success, -1 on bad magic.
 */
static int parse_header(usize dtb_pa, FdtHeader* out) {
    const unsigned char* base = (const unsigned char*)dtb_pa;

    if (read_be32(base + 0) != FDT_MAGIC) {
        return -1;
    }

    out->off_struct = read_be32(base + 0x08);
    out->off_strings = read_be32(base + 0x0C);
    return 0;
}

/**
 * @brief Return the NUL-terminated property name at @p nameoff .
 *
 * @param base    Pointer to the DTB base.
 * @param header  Parsed header fields.
 * @param nameoff Offset from strings block to property name.
 * @return The property name.
 */
static const char* fdt_string(
    const unsigned char* base, const FdtHeader* header, unsigned int nameoff
) {
    return (const char*)(base + header->off_strings + nameoff);
}

/**
 * @brief Compare two NUL-terminated strings for equality.
 *
 * @param a Pointer to the first string.
 * @param b Pointer to the second string.
 * @return true if the strings are equal, false otherwise.
 */
static bool str_eq(const char* a, const char* b) {
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return false;
        }
        a++;
        b++;
    }
    return *a == *b;
}

/**
 * @brief Walk the structure block and look for `timebase-frequency`.
 *
 * @param base DTB base pointer.
 * @param header Parsed header.
 * @return The property value, or 0 if not found.
 */
static unsigned int find_timebase_freq(const unsigned char* base, const FdtHeader* header) {
    const unsigned char* ptr = base + header->off_struct;
    int depth = 0;

    while (true) {
        unsigned int token = read_be32(ptr);
        ptr += 4;

        if (token == FDT_BEGIN_NODE) {
            while (*ptr != 0) {
                ptr++;
            }
            ptr++;
            ptr = (const unsigned char*)(((usize)ptr + 3) & ~(usize)3);
            depth++;
        } else if (token == FDT_END_NODE) {
            depth--;
            if (depth == 0) {
                return 0;
            }
        } else if (token == FDT_PROP) {
            unsigned int len = read_be32(ptr);
            unsigned int nameoff = read_be32(ptr + 4);
            ptr += 8;
            const char* name = fdt_string(base, header, nameoff);
            if (str_eq(name, "timebase-frequency")) {
                if (len != 4) {
                    return 0;
                }
                return read_be32(ptr);
            }
            ptr += len;
            ptr = (const unsigned char*)(((usize)ptr + 3) & ~(usize)3);
        } else if (token == FDT_NOP) {
            // skip
        } else if (token == FDT_END) {
            return 0;
        } else {
            return 0;
        }
    }
}

int device_tree_init(usize dtb_pa) {
    const unsigned char* base = (const unsigned char*)dtb_pa;
    FdtHeader header;

    if (parse_header(dtb_pa, &header) != 0) {
        error("[kernel] Invalid device tree magic.");
        return -1;
    }

    unsigned int freq = find_timebase_freq(base, &header);
    if (freq == 0) {
        error("[kernel] Timebase-frequency property not found in device tree.");
        return -1;
    }

    global_clock_freq = (usize)freq;
    info("[kernel] CLOCK_FREQ from device tree: %lu", (unsigned long)global_clock_freq);
    return 0;
}

usize device_tree_clock_freq() {
    assert_panic(global_clock_freq != 0);
    return global_clock_freq;
}
