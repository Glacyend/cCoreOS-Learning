/**
 * @file config.c
 * @brief Constants used in cCoreOS.
 */

#include "config.h"

void kernel_stack_position(usize app_id, usize* bottom, usize* top) {
    *top = TRAMPOLINE - app_id * (KERNEL_STACK_SIZE + PAGE_SIZE);
    *bottom = *top - KERNEL_STACK_SIZE;
}
