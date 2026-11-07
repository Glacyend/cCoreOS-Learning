/**
 * @file syscall.c
 * @brief System call dispatcher.
 */

#include "syscall.h"
#include "../config.h"
#include "../panic.h"
#include "fs.h"
#include "process.h"

isize syscall_dispatch(usize id, usize arg0, usize arg1, usize arg2) {
    switch (id) {
        case SYSCALL_WRITE: {
            return sys_write(arg0, (const char*)arg1, arg2);
        }
        case SYSCALL_EXIT: {
            sys_exit((int)arg0);
        }
        case SYSCALL_YIELD: {
            // TODO
            return 0;
        }
        case SYSCALL_GET_TIME: {
            // TODO
            return 0;
        }
        default: {
            panic("[kernel] Unsupported syscall id: %lu", (unsigned long)id);
        }
    }
}
