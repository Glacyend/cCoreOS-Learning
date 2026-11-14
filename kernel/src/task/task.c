/**
 * @file task.c
 * @brief Task management implementation.
 */

#include "task.h"
#include "../loader.h"
#include "../logging.h"
#include "../mm/slice.h"
#include "../mm/vec.h"
#include "../panic.h"
#include "../sbi.h"
#include "../trap/context.h"
#include "context.h"
#include "switch.h"
#include "task_inner.h"

/**
 * @brief The task manager inner.
 *
 * Rust equivalent: `TaskManagerInner`.
 */
typedef struct {
    /** @brief Task list. */
    Vector tasks;

    /** @brief Id of current `TASK_STATUS_RUNNING` task. */
<<<<<<< Updated upstream
    usize current_task;
=======
    size_t current_task;
>>>>>>> Stashed changes
} TaskManagerInner;

/**
 * @brief The task manager, where all the tasks are managed.
 *
 * Rust equivalent: `TaskManager`. In C, the `UPSafeCell` wrapper is dropped
 * and the inner value is accessed directly; this is safe on a single core with
 * cooperative scheduling.
 */
typedef struct {
    /** @brief Total number of tasks. */
<<<<<<< Updated upstream
    usize num_app;
=======
    size_t num_app;
>>>>>>> Stashed changes

    /** @brief Inner value. */
    TaskManagerInner inner;
} TaskManager;

/**
 * @brief Global task manager.
 *
 * Rust equivalent: `TASK_MANAGER`, lazily initialized by `lazy_static!`. C
 * requires an explicit @ref task_init call before use.
 */
static TaskManager global_task_manager;
static bool global_task_manager_initialized = false;

/**
 * @brief Find the next to run and return task id.
 *
 * In this case, we only return the first `TASK_STATUS_READY` task in the task
 * list.
 *
 * Rust equivalent: `TaskManager::find_next_task`.
 */
<<<<<<< Updated upstream
static void task_manager_find_next_task(TaskManager* manager, bool* found, usize* out) {
    usize current = manager->inner.current_task;
    usize num = manager->num_app;
    for (usize i = 1; i <= num; i++) {
        usize id = (current + i) % num;
=======
static void task_manager_find_next_task(TaskManager* manager, bool* found, size_t* out) {
    size_t current = manager->inner.current_task;
    size_t num = manager->num_app;
    for (size_t i = 1; i <= num; i++) {
        size_t id = (current + i) % num;
>>>>>>> Stashed changes
        TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, id);
        if (task->task_status == TASK_STATUS_READY) {
            *found = true;
            *out = id;
            return;
        }
    }
    *found = false;
}

/**
 * @brief Change the status of current `TASK_STATUS_RUNNING` task into
 *        `TASK_STATUS_READY`.
 *
 * Rust equivalent: `TaskManager::mark_current_suspended`.
 */
static void task_manager_mark_current_suspended(TaskManager* manager) {
<<<<<<< Updated upstream
    usize current = manager->inner.current_task;
=======
    size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
    TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
    task->task_status = TASK_STATUS_READY;
}

/**
 * @brief Change the status of current `TASK_STATUS_RUNNING` task into
 *        `TASK_STATUS_READY`.
 *
 * Rust equivalent: `TaskManager::mark_current_exited`.
 */
static void task_manager_mark_current_exited(TaskManager* manager) {
<<<<<<< Updated upstream
    usize current = manager->inner.current_task;
=======
    size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
    TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
    task->task_status = TASK_STATUS_EXITED;
}

/**
 * @brief Get the current `TASK_STATUS_RUNNING` task's token.
 *
 * Rust equivalent: `TaskManager::get_current_token`.
 */
<<<<<<< Updated upstream
static usize task_manager_get_current_token(TaskManager* manager) {
    usize current = manager->inner.current_task;
=======
static size_t task_manager_get_current_token(TaskManager* manager) {
    size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
    TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
    return task_control_block_get_user_token(task);
}

/**
 * @brief Get the current `TASK_STATUS_RUNNING` task's `TrapContext`.
 *
 * Rust equivalent: `TaskManager::get_current_trap_cx`.
 */
static TrapContext* task_manager_get_current_trap_cx(TaskManager* manager) {
<<<<<<< Updated upstream
    usize current = manager->inner.current_task;
=======
    size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
    TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
    return task_control_block_get_trap_cx(task);
}

/**
 * @brief Change the current `TASK_STATUS_RUNNING` task's program break.
 *
 * Rust equivalent: `TaskManager::change_current_program_brk`.
 */
static bool task_manager_change_current_program_brk(
<<<<<<< Updated upstream
    TaskManager* manager, int32_t size, usize* out
) {
    usize current = manager->inner.current_task;
=======
    TaskManager* manager, int32_t size, size_t* out
) {
    size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
    TaskControlBlock* task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
    return task_control_block_change_program_brk(task, size, out);
}

/**
 * @brief Switch current `TASK_STATUS_RUNNING` task to the task we have found,
 *        or there is no `TASK_STATUS_READY` task and we can exit with all
 *        applications completed.
 *
 * Rust equivalent: `TaskManager::run_next_task`.
 */
static void task_manager_run_next_task(TaskManager* manager) {
    bool found;
<<<<<<< Updated upstream
    usize next;
    task_manager_find_next_task(manager, &found, &next);
    if (found) {
        usize current = manager->inner.current_task;
=======
    size_t next;
    task_manager_find_next_task(manager, &found, &next);
    if (found) {
        size_t current = manager->inner.current_task;
>>>>>>> Stashed changes
        TaskControlBlock* next_task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, next);
        next_task->task_status = TASK_STATUS_RUNNING;
        manager->inner.current_task = next;
        TaskControlBlock* current_task =
            (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, current);
        __switch(&current_task->task_cx, &next_task->task_cx);
        // go back to user mode
    } else {
        info("[kernel] All applications completed!");
        sbi_shutdown(false);
    }
}

/**
 * @brief Run the first task in task list.
 *
 * Rust equivalent: `TaskManager::run_first_task`.
 */
[[noreturn]] static void task_manager_run_first_task(TaskManager* manager) {
    TaskControlBlock* first_task = (TaskControlBlock*)vec_get_mut(&manager->inner.tasks, 0);
    first_task->task_status = TASK_STATUS_RUNNING;
    TaskContext unused = task_context_zero_init();
    __switch(&unused, &first_task->task_cx);
    // unreachable
    __builtin_unreachable();
}

void task_init() {
    if (global_task_manager_initialized) {
        return;
    }
    trace("init TASK_MANAGER");
<<<<<<< Updated upstream
    usize num_app = loader_get_num_app();
    trace("num_app = %zu", num_app);
    global_task_manager.num_app = num_app;
    vec_init(&global_task_manager.inner.tasks, sizeof(TaskControlBlock));
    for (usize i = 0; i < num_app; i++) {
=======
    size_t num_app = loader_get_num_app();
    trace("num_app = %zu", num_app);
    global_task_manager.num_app = num_app;
    vec_init(&global_task_manager.inner.tasks, sizeof(TaskControlBlock));
    for (size_t i = 0; i < num_app; i++) {
>>>>>>> Stashed changes
        ByteSlice elf = loader_get_app_data(i);
        TaskControlBlock task;
        task_control_block_new(&task, elf.data, elf.len, i);
        if (!vec_push(&global_task_manager.inner.tasks, &task)) {
            panic("task_init: out of memory");
        }
    }
    global_task_manager.inner.current_task = 0;
    global_task_manager_initialized = true;
}

[[noreturn]] void task_run_first_task() {
    task_manager_run_first_task(&global_task_manager);
}

void task_run_next_task() {
    task_manager_run_next_task(&global_task_manager);
}

void task_suspend_current_and_run_next() {
    task_manager_mark_current_suspended(&global_task_manager);
    task_run_next_task();
}

[[noreturn]] void task_exit_current_and_run_next() {
    task_manager_mark_current_exited(&global_task_manager);
    task_run_next_task();
    // unreachable
    __builtin_unreachable();
}

<<<<<<< Updated upstream
usize task_current_user_token() {
=======
size_t task_current_user_token() {
>>>>>>> Stashed changes
    return task_manager_get_current_token(&global_task_manager);
}

TrapContext* task_current_trap_cx() {
    return task_manager_get_current_trap_cx(&global_task_manager);
}

<<<<<<< Updated upstream
bool task_change_current_program_brk(int32_t size, usize* out) {
=======
bool task_change_current_program_brk(int size, size_t* out) {
>>>>>>> Stashed changes
    return task_manager_change_current_program_brk(&global_task_manager, size, out);
}
