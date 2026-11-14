/**
 * @file fs.h
 * @brief File and filesystem-related syscalls.
 */

#ifndef __SYSCALL_FS_H_
#define __SYSCALL_FS_H_

#include <stddef.h>

/**
 * @brief Write `buf` of length `len` to a file with `fd`.
 *
 * Rust equivalent: `sys_write`. Only `fd == 1` (stdout) is supported.
 *
 * The buffer is translated page by page through the current task's page table,
 * because the kernel runs under the kernel page table while `buf` is a user
 * virtual address.
 *
 * @param fd  File descriptor.
 * @param buf Buffer to read from.
 * @param len Number of bytes to read.
 * @return `len` on success, or -1 on error.
 */
<<<<<<< Updated upstream
isize sys_write(usize fd, const uint8_t* buf, usize len);
=======
long sys_write(size_t fd, const char* buf, size_t len);
>>>>>>> Stashed changes

#endif /* __SYSCALL_FS_H_ */
