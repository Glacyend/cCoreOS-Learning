/**
 * @file process.c
 * @brief Process-management syscalls.
 */

#include "process.h"
#include "../logging.h"
#include "../task/task.h"

[[noreturn]] void sys_exit(int exit_code) {
    info("[kernel] Application exited with code %d.", exit_code);

    task_run_next_app();
}
