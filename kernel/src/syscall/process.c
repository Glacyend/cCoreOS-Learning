/**
 * @file process.c
 * @brief Process management syscalls.
 */

#include "process.h"
#include "../logging.h"
#include "../task/task.h"
#include "../timer.h"

[[noreturn]] void sys_exit(int32_t exit_code) {
    info("[kernel] Application exited with code %d.", exit_code);
    task_exit_current_and_run_next();
}

isize sys_yield() {
    task_suspend_current_and_run_next();
    return 0;
}

isize sys_get_time() {
    return (isize)timer_get_time_ms();
}

isize sys_sbrk(int32_t size) {
    usize old_brk;
    if (task_change_current_program_brk(size, &old_brk)) {
        return (isize)old_brk;
    } else {
        return -1;
    }
}
