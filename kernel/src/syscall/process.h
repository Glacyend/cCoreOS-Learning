/**
 * @file process.h
 * @brief Process-management syscalls.
 */

#ifndef __SYSCALL_PROCESS_H_
#define __SYSCALL_PROCESS_H_

/**
 * @brief Terminate the current application.
 *
 * @param exit_code Exit code reported in the log.
 * @return Never returns.
 */
[[noreturn]] void sys_exit(int exit_code);

#endif /* __SYSCALL_PROCESS_H_ */
