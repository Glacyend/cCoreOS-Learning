/**
 * @file fs.h
 * @brief File and filesystem-related syscalls.
 */

#ifndef __SYSCALL_FS_H_
#define __SYSCALL_FS_H_

#include "../config.h"

/**
 * @brief Write @p len bytes from @p buf to file descriptor @p fd .
 *
 * Only `fd == 1` (stdout) is supported.
 *
 * @param fd  File descriptor.
 * @param buf Buffer to read from.
 * @param len Number of bytes to read.
 * @return Number of bytes written, or -1 an error.
 */
isize sys_write(usize fd, const char* buf, usize len);

#endif /* __SYSCALL_FS_H_ */
