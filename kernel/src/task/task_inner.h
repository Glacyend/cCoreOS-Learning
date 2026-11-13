/**
 * @file task_inner.h
 * @brief Types related to task management.
 */

#ifndef __TASK_TASK_INNER_H_
#define __TASK_TASK_INNER_H_

#include "../config.h"
#include "../mm/address.h"
#include "../mm/memory_set.h"
#include "../trap/context.h"
#include "context.h"

/**
 * @brief Task status.
 *
 * Rust equivalent: `TaskStatus`.
 *
 * - `TASK_STATUS_READY`
 * - `TASK_STATUS_RUNNING`
 * - `TASK_STATUS_EXITED`
 */
typedef enum {
    TASK_STATUS_READY,
    TASK_STATUS_RUNNING,
    TASK_STATUS_EXITED,
} TaskStatus;

/**
 * @brief Task control block structure.
 *
 * Rust equivalent: `TaskControlBlock`.
 */
typedef struct {
    TaskStatus task_status;
    TaskContext task_cx;
    MemorySet memory_set;
    PhysPageNum trap_cx_ppn;
    usize base_size;
    usize heap_bottom;
    usize program_brk;
} TaskControlBlock;

/**
 * @brief Return the `TrapContext` of this task.
 *
 * Rust equivalent: `TaskControlBlock::get_trap_cx`. The `TrapContext` lives in
 * the task's user address space, in the page whose physical number is
 * @ref TaskControlBlock.trap_cx_ppn .
 */
TrapContext* task_control_block_get_trap_cx(const TaskControlBlock* block);

/**
 * @brief Return the `satp` token of this task's memory set.
 *
 * Rust equivalent: `TaskControlBlock::get_user_token`.
 */
usize task_control_block_get_user_token(const TaskControlBlock* block);

/**
 * @brief Construct a `TaskControlBlock` from an ELF image.
 *
 * Creates the task's `MemorySet`, maps a kernel stack for it, and initializes
 * its `TrapContext`.
 *
 * Rust equivalent: `TaskControlBlock::new`.
 *
 * @param block    Task control block to initialize.
 * @param elf_data ELF bytes.
 * @param elf_len  Length of @p elf_data .
 * @param app_id   App index, used to place the kernel stack.
 */
void task_control_block_new(
    TaskControlBlock* block, const uint8_t* elf_data, usize elf_len, usize app_id
);

/**
 * @brief Change the location of the program break.
 *
 * Rust equivalent: `TaskControlBlock::change_program_brk`.
 *
 * @param block Task control block.
 * @param size  Signed delta for the program break.
 * @param out   Receives the old program break on success.
 * @return `true` on success, `false` if the request is invalid.
 */
bool task_control_block_change_program_brk(TaskControlBlock* block, int32_t size, usize* out);

#endif /* __TASK_TASK_INNER_H_ */
