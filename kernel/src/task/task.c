/**
 * @file task.c
 * @brief Task management: cooperative round-robin scheduler.
 */

#include "task.h"
#include "../config.h"
#include "../loader.h"
#include "../logging.h"
#include "../sbi.h"
#include "context.h"
#include "switch.h"

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_EXITED,
} TaskStatus;

typedef struct {
    TaskStatus status;
    TaskContext cx;
} TaskControlBlock;

typedef struct {
    usize num_app;
    usize current_task;
    TaskControlBlock tasks[MAX_APP_NUM];
} TaskManager;

static TaskManager global_task_manager;

/** @brief Provided by trap.S. */
extern char __restore[];

static TaskContext tc_goto_restore(usize cx_addr) {
    TaskContext cx;
    cx.ra = (usize)__restore;
    cx.sp = cx_addr;
    for (int i = 0; i < 12; i++) {
        cx.s[i] = 0;
    }
    return cx;
}

void task_init() {
    loader_load_apps();

    global_task_manager.num_app = loader_get_num_app();
    global_task_manager.current_task = 0;

    for (usize i = 0; i < global_task_manager.num_app; i++) {
        usize cx_addr = loader_init_app_cx(i);
        global_task_manager.tasks[i].cx = tc_goto_restore(cx_addr);
        global_task_manager.tasks[i].status = TASK_READY;
    }

    trace("[kernel] task_init: %lu task(s) ready.", (unsigned long)global_task_manager.num_app);
}

static bool find_next_task(usize* out) {
    usize current = global_task_manager.current_task;
    usize num = global_task_manager.num_app;
    for (usize i = 1; i <= num; i++) {
        usize id = (current + i) % num;
        if (global_task_manager.tasks[id].status == TASK_READY) {
            *out = id;
            return true;
        }
    }
    return false;
}

static void run_next_task() {
    usize next;
    if (!find_next_task(&next)) {
        info("[kernel] All applications completed.");
        sbi_shutdown(false);
    }

    usize current = global_task_manager.current_task;
    global_task_manager.tasks[next].status = TASK_RUNNING;
    global_task_manager.current_task = next;

    TaskContext* current_cx = &global_task_manager.tasks[current].cx;
    TaskContext* next_cx = &global_task_manager.tasks[next].cx;

    __switch(current_cx, next_cx);
}

[[noreturn]] void run_first_task() {
    global_task_manager.tasks[0].status = TASK_RUNNING;

    TaskContext unused;
    unused.ra = 0;
    unused.sp = 0;
    for (int i = 0; i < 12; i++) {
        unused.s[i] = 0;
    }

    TaskContext* first_cx = &global_task_manager.tasks[0].cx;
    __switch(&unused, first_cx);

    __builtin_unreachable();
}

void task_suspend_current_and_run_next() {
    global_task_manager.tasks[global_task_manager.current_task].status = TASK_READY;
    run_next_task();
}

[[noreturn]] void task_exit_current_and_run_next() {
    global_task_manager.tasks[global_task_manager.current_task].status = TASK_EXITED;
    run_next_task();
    __builtin_unreachable();
}
