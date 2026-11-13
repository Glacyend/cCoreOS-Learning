/**
 * @file task_inner.c
 * @brief Types related to task management.
 */

#include "task_inner.h"
#include "../panic.h"
#include "../trap/context.h"

#include <stddef.h>

/**
 * @brief Address of `trap_handler`, provided by `trap.C`.
 *
 * Rust equivalent: `linker_symbol_addr!(trap::trap_handler)`.
 */
extern void trap_handler();

TrapContext* task_control_block_get_trap_cx(const TaskControlBlock* block) {
    return (TrapContext*)phys_page_num_get_mut(block->trap_cx_ppn);
}

usize task_control_block_get_user_token(const TaskControlBlock* block) {
    return memory_set_token(&block->memory_set);
}

void task_control_block_new(
    TaskControlBlock* block, const uint8_t* elf_data, usize elf_len, usize app_id
) {
    // memory_set with elf program headers / trampoline / trap_context / user
    // stack
    usize user_sp;
    usize entry_point;
    memory_set_from_elf(&block->memory_set, elf_data, elf_len, &user_sp, &entry_point);

    VirtPageNum trap_cx_vpn = virt_page_num_from_virt_addr(virt_addr_from_usize(TRAP_CONTEXT));
    PageTableEntry trap_cx_pte;
    if (!memory_set_translate(&block->memory_set, trap_cx_vpn, &trap_cx_pte)) {
        panic("task_control_block_new: TRAP_CONTEXT is not mapped");
    }
    PhysPageNum trap_cx_ppn = page_table_entry_ppn(&trap_cx_pte);

    block->task_status = TASK_STATUS_READY;

    // map a kernel-stack in kernel space
    usize kernel_stack_bottom;
    usize kernel_stack_top;
    kernel_stack_position(app_id, &kernel_stack_bottom, &kernel_stack_top);
    memory_set_insert_framed_area(
        kernel_space(),
        virt_addr_from_usize(kernel_stack_bottom),
        virt_addr_from_usize(kernel_stack_top),
        MapPermission_R | MapPermission_W
    );

    block->task_cx = task_context_goto_trap_return(kernel_stack_top);
    block->trap_cx_ppn = trap_cx_ppn;
    block->base_size = user_sp;
    block->heap_bottom = user_sp;
    block->program_brk = user_sp;

    // prepare `TrapContext` in user space
    TrapContext* trap_cx = task_control_block_get_trap_cx(block);
    *trap_cx = trap_context_app_init_context(
        entry_point,
        user_sp,
        memory_set_token(kernel_space()),
        kernel_stack_top,
        (usize)trap_handler
    );
}

bool task_control_block_change_program_brk(TaskControlBlock* block, int32_t size, usize* out) {
    usize old_break = block->program_brk;
    isize new_brk = (isize)block->program_brk + (isize)size;
    if (new_brk < (isize)block->heap_bottom) {
        return false;
    }
    bool result;
    if (size < 0) {
        result = memory_set_shrink_to(
            &block->memory_set,
            virt_addr_from_usize(block->heap_bottom),
            virt_addr_from_usize((usize)new_brk)
        );
    } else {
        result = memory_set_append_to(
            &block->memory_set,
            virt_addr_from_usize(block->heap_bottom),
            virt_addr_from_usize((usize)new_brk)
        );
    }
    if (result) {
        block->program_brk = (usize)new_brk;
        if (out != NULL) {
            *out = old_break;
        }
        return true;
    } else {
        return false;
    }
}
