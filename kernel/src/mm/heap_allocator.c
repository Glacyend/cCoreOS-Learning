/**
 * @file heap_allocator.c
 * @brief The global allocator.
 *
 * Rust uses `buddy_system_allocator::LockedHeap<32>`. The C version uses a
 * free-list first-fit allocator with immediate coalescing: each block carries
 * a `Block` header, free blocks are linked, and neighboring free blocks are
 * merged on `kfree`. This is simpler than buddy and sufficient for a teaching
 * kernel.
 */

#include "heap_allocator.h"
#include "../panic.h"

#include <stddef.h>

/**
 * @brief Alignment of every allocation (16 bytes).
 */
#define HEAP_ALIGN 16

/**
 * @brief Round @p x up to a multiple of @ref HEAP_ALIGN .
 */
#define ALIGN_UP(x) (((x) + (HEAP_ALIGN - 1)) & ~((usize)(HEAP_ALIGN - 1)))

/**
 * @brief Header prepended to every block.
 */
typedef struct Block {
    /** @brief Total block size, including this header. */
    usize size;

    /** @brief Whether the block is currently free. */
    bool free;

    /** @brief Next physically adjacent block. */
    struct Block* next;
    
    /** @brief Previous physically adjacent block. */
    struct Block* prev;
} Block;

/**
 * @brief Heap space (`[u8; KERNEL_HEAP_SIZE]`), 4 KiB-aligned.
 */
static alignas(4096) uint8_t global_heap_space[KERNEL_HEAP_SIZE];

/**
 * @brief Head of the block list.
 */
static Block* global_heap_head = NULL;

void init_heap() {
    global_heap_head = (Block*)global_heap_space;
    global_heap_head->size = KERNEL_HEAP_SIZE;
    global_heap_head->free = true;
    global_heap_head->next = NULL;
    global_heap_head->prev = NULL;
}

/**
 * @brief Split @p block if the trailing part can fit another block.
 *
 * The first @p needed bytes stay allocated; the remainder becomes a new free
 * block linked after @p block .
 */
static void heap_split(Block* block, usize needed) {
    if (block->size - needed < sizeof(Block) + HEAP_ALIGN) {
        return;
    }
    Block* rest = (Block*)((uint8_t*)block + needed);
    rest->size = block->size - needed;
    rest->free = true;
    rest->next = block->next;
    rest->prev = block;
    if (block->next != NULL) {
        block->next->prev = rest;
    }
    block->next = rest;
    block->size = needed;
}

void* kmalloc(usize size) {
    if (size == 0) {
        return NULL;
    }
    usize needed = sizeof(Block) + ALIGN_UP(size);

    Block* block = global_heap_head;
    while (block != NULL) {
        if (block->free && block->size >= needed) {
            heap_split(block, needed);
            block->free = false;
            return (void*)((uint8_t*)block + sizeof(Block));
        }
        block = block->next;
    }
    return NULL;
}

/**
 * @brief Merge @p block with its free neighbors.
 */
static void heap_coalesce(Block* block) {
    if (block->next != NULL && block->next->free) {
        Block* next = block->next;
        block->size += next->size;
        block->next = next->next;
        if (block->next != NULL) {
            block->next->prev = block;
        }
    }
    if (block->prev != NULL && block->prev->free) {
        Block* prev = block->prev;
        prev->size += block->size;
        prev->next = block->next;
        if (prev->next != NULL) {
            prev->next->prev = prev;
        }
    }
}

void kfree(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((uint8_t*)ptr - sizeof(Block));
    if (block->free) {
        panic("kfree: double free at %p", ptr);
    }
    block->free = true;
    heap_coalesce(block);
}
