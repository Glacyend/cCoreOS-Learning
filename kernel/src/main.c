/**
 * @file main.c
 * @brief Kernel entry point and high-level initialization.
 *
 * Reached from `entry.S` after the boot stack is set up. The order of
 * initialization mirrors the Rust version:
 *
 *   clear_bss() -> logging -> device_tree -> mm -> trap -> timer -> task_init
 *
 * Cooperative round-robin scheduler. Every app is loaded once at startup, then
 * tasks switch on `sys_yield`/`sys_exit`.
 */

#include "config.h"
#include "console.h"
#include "device_tree.h"
#include "logging.h"
#include "panic.h"
#include "task/task.h"
#include "timer.h"
#include "trap/trap.h"

// Symbols defined by linker.ld.
extern char sbss[];
extern char ebss[];

/**
 * @brief Clear the kernel .bss segment (excluding the boot stack).
 *
 * The boot stack lives in .bss.stack, which is placed *before* `sbss` by the
 * linker script, so this loop never touches the stack we are currently running
 * on.
 */
static void clear_bss() {
    for (char* p = sbss; p < ebss; p++) {
        *p = 0;
    }
}

/**
 * @brief Rust-style kernel entry point.
 *
 * @param hart_id Hard id passed by OpenSBI in a0.
 * @param dtb_pa  Physical address of the device tree passed in a1.
 * @return Never returns.
 */
[[noreturn]] void kernel_main([[maybe_unused]] usize hart_id, [[maybe_unused]] usize dtb_pa) {
    clear_bss();
    log_init();

    trace("[kernel] Start running.");

    const char* banner =
        "  ______   ______   ______   .______       _______   ______        _______.\n"
        " /      | /      | /  __  \\  |   _  \\     |   ____| /  __  \\      /       |\n"
        "|  ,----'|  ,----'|  |  |  | |  |_)  |    |  |__   |  |  |  |    |   (----`\n"
        "|  |     |  |     |  |  |  | |      /     |   __|  |  |  |  |     \\   \\\n"
        "|  `----.|  `----.|  `--'  | |  |\\  \\----.|  |____ |  `--'  | .----)   |\n"
        " \\______| \\______| \\______/  | _| `._____||_______| \\______/  |_______/\n";

    println("%s", banner);

    println("[kernel] Welcome to cCoreOS!");

    if (device_tree_init(dtb_pa) != 0) {
        panic("[kernel] Device tree init failed.");
    }

    trap_init();
    trap_enable_timer_interrupt();

    timer_set_next_trigger();

    task_init();
    run_first_task();
}
