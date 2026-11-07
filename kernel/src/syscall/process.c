/**
 * @file process.c
 * @brief Process-management syscalls.
 */

#include "process.h"
#include "../logging.h"
#include "../sbi.h"

[[noreturn]] void sys_exit(int exit_code) {
    info("[kernel] Application exited with code %d.", exit_code);

    // TODO
    sbi_shutdown(false);
}
