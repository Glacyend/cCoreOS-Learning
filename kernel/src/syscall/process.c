/**
 * @file process.c
 * @brief Process-management syscalls.
 */

#include "process.h"
#include "../logging.h"
#include "../task/task.h"

[[noreturn]] void sys_exit(int exit_code) {
    info("[kernel] Application exited with code %d.", exit_code);
    task_exit_current_and_run_next();
}

isize sys_yield() {
    task_suspend_current_and_run_next();
    return 0;
}
