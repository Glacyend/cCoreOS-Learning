/**
 * @file syscall.c
 * @brief Implementation of syscalls.
 */

#include "syscall.h"
#include "../panic.h"
#include "fs.h"
#include "process.h"

<<<<<<< Updated upstream
isize syscall_dispatch(usize syscall_id, usize arg0, usize arg1, usize arg2) {
=======
long syscall_dispatch(size_t syscall_id, size_t arg0, size_t arg1, size_t arg2) {
>>>>>>> Stashed changes
    switch (syscall_id) {
        case SYSCALL_WRITE: {
            return sys_write(arg0, (const uint8_t*)arg1, arg2);
        }
        case SYSCALL_EXIT: {
            sys_exit((int32_t)arg0);
        }
        case SYSCALL_YIELD: {
            return sys_yield();
        }
        case SYSCALL_GET_TIME: {
            return sys_get_time();
        }
        case SYSCALL_SBRK: {
            return sys_sbrk((int32_t)arg0);
        }
        default: {
            panic("Unsupported syscall_id: %zu", syscall_id);
        }
    }
}
