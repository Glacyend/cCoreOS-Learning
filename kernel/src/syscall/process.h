/**
 * @file process.h
 * @brief Process-management syscalls.
 */

#ifndef __SYSCALL_PROCESS_H_
#define __SYSCALL_PROCESS_H_

#include "../config.h"

/**
 * @brief Terminate the current task.
 *
 * Marks the task Exited and switches to the next Ready task. The current task
 * is never scheduled again, so this function does not return.
 *
 * @param exit_code Exit code reported in the log.
 * @return Never returns.
 */
[[noreturn]] void sys_exit(int exit_code);

/**
 * @brief Yield the CPU to the next Ready task.
 *
 * @return 0, once the current task is scheduled again.
 */
isize sys_yield();

#endif /* __SYSCALL_PROCESS_H_ */
