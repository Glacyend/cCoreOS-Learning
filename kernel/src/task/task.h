/**
 * @file task.h
 * @brief Batch scheduler: run every embedded app in order.
 */

#ifndef __TASK_H_
#define __TASK_H_

/**
 * @brief Load and run the next app; shut down when all are done.
 *
 * @return Never returns.
 */
[[noreturn]] void task_run_next_app();

#endif /* __TASK_H_ */
