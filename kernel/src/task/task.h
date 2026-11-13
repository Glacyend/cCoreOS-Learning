/**
 * @file task.h
 * @brief Task management implementation.
 *
 * Everything about task management, like starting and switching tasks is
 * implemented here.
 *
 * A single global instance of `TaskManager` called `TASK_MANAGER` controls all
 * the tasks in the operating system.
 *
 * Be careful when you see `__switch` ASM function in `switch.S`. Control flow
 * around this function might not be what you expect.
 */

#ifndef __TASK_H_
#define __TASK_H_

#include "../config.h"
#include "../trap/context.h"

/**
 * @brief Initialize the global task manager.
 *
 * Rust equivalent: the `lazy_static!` initializer for `TASK_MANAGER`. Must be
 * called once before any other function in this module.
 */
void task_init();

/**
 * @brief Run the first task in task list.
 *
 * Generally, the first task in task list is an idle task (we call it zero
 * process later). But for now, we load apps statically, so the first task is a
 * real app.
 *
 * Rust equivalent: `run_first_task`.
 */
[[noreturn]] void task_run_first_task();

/**
 * @brief Switch current `TASK_STATUS_RUNNING` task to the task we have found,
 *        or there is no `TASK_STATUS_READY` task and we can exit with all
 *        applications completed.
 *
 * Rust equivalent: `run_next_task`.
 */
void task_run_next_task();

/**
 * @brief Suspend the current `TASK_STATUS_RUNNING` task and run the next task
 *        in task list.
 *
 * Rust equivalent: `suspend_current_and_run_next`.
 */
void task_suspend_current_and_run_next();

/**
 * @brief Exit the current `TASK_STATUS_RUNNING` task and run the next task in
 *        task list.
 *
 * Rust equivalent: `exit_current_and_run_next`.
 */
[[noreturn]] void task_exit_current_and_run_next();

/**
 * @brief Get the current `TASK_STATUS_RUNNING` task's token.
 *
 * Rust equivalent: `current_user_token`.
 */
usize task_current_user_token();

/**
 * @brief Get the current `TASK_STATUS_RUNNING` task's `TrapContext`.
 *
 * Rust equivalent: `current_trap_cx`.
 */
TrapContext* task_current_trap_cx();

/**
 * @brief Change the current `TASK_STATUS_RUNNING` task's program break.
 *
 * Rust equivalent: `change_current_program_brk`.
 *
 * @param size Signed dalta for the program break.
 * @param out  Receives the old program break on success.
 * @return `true` on success, `false` if the request is invalid.
 */
bool task_change_current_program_brk(int32_t size, usize* out);

#endif /* __TASK_H_ */
