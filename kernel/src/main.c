/**
 * @file main.c
 * @brief The main module and entrypoint.
 *
 * Various facilities of the kernels are implemented as submodules. The most
 * important ones are:
 *
 * - `trap`: Handles all cases of switching from userspace to the kernel.
 * - `task`: Task management.
 * - `syscall`: System call handling and implementation.
 *
 * The operating system also starts in this module. Kernel code starts
 * executing from `entry.S`, after which `kernel_main()` is called to
 * initialize various pieces of functionality. (See its source code for
 * details.)
 *
 * We then call `task_run_first_task()` and for the first time go to userspace.
 */

#include "console.h"
#include "device_tree.h"
#include "logging.h"
<<<<<<< Updated upstream
#include "mm/heap_allocator.h"
#include "panic.h"
=======
#include "mm/mm.h"
>>>>>>> Stashed changes
#include "task/task.h"
#include "timer.h"
#include "trap/trap.h"

// Symbols defined by linker.ld.
extern char sbss[];
extern char ebss[];

/**
 * @brief Clear BSS segment.
 *
 * Rust equivalent: `clear_bss`. `sbss` and `ebss` are linker script symbols;
 * the range `sbss..ebss` excludes `.bss.stack`, so the boot stack in use is
 * not zeroed.
 */
static void clear_bss() {
    for (char* p = sbss; p < ebss; p++) {
        *(volatile char*)p = 0;
    }
}

/**
 * @brief The C entry-point of OS.
 *
 * Rust equivalent: `rust_main`. `entry.S` calls this after setting `sp` to
 * `boot_stack_top`.
 *
 * @param hart_id Hard id passed by OpenSBI in `a0`.
 * @param dtb_pa  Physical address of the device tree passed in `a1`.
 */
[[noreturn]] void kernel_main([[maybe_unused]] size_t hart_id, [[maybe_unused]] size_t dtb_pa) {
    clear_bss();
    init_heap();
    log_init();
    device_tree_init(dtb_pa);

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

    mm_init();
    trap_init();
    // trap_enable_interrupt();
    trap_enable_timer_interrupt();
    timer_set_next_trigger();
    task_init();
    task_run_first_task();
}
