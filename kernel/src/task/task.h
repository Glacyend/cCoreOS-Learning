/**
 * @file task.h
 * @brief Task management: cooperative round-robin scheduler.
 */

#ifndef __TASK_H_
#define __TASK_H_

/**
 * @brief Initialize the task manager.
 *
 * Loads every embedded app, builds one `TaskControlBlock` per app, and make
 * all of them Ready.
 */
void task_init();

/**
 * @brief Run the first task; never returns to the caller.
 *
 * @return Never returns.
 */
[[noreturn]] void run_first_task();

/**
 * @brief Suspend the current task and switch to the next Ready task.
 *
 * When the current task is scheduled again, this function returns normally so
 * the caller (e.g. `sys_yield`) can proceed.
 */
void task_suspend_current_and_run_next();

/**
 * @brief Mark the current task Exited and switch to the next Ready one.
 *
 * The current task is never scheduled again, so this function does not return.
 *
 * @return Never returns.
 */
[[noreturn]] void task_exit_current_and_run_next();

#endif /* __TASK_H_ */
