/**
 * @file context.h
 * @brief Task context: callee-saved registers saved by `__switch`.
 *
 * Layout matches `switch.S` exactly:
 *
 *   offset 0      ra
 *   offset 8      sp
 *   offset 16+8n  s[n], n = 0..11
 *
 * Total size: 14 * 8 = 112 bytes.
 */

#ifndef __TASK_CONTEXT_H_
#define __TASK_CONTEXT_H_

#include "../config.h"

/**
 * @brief Task context.
 *
 * `ra` is the address `__switch` returns to:
 *   - `__restore` for a task that has never run before;
 *   - the instruction after `__switch` for a task that was preempted.
 */
typedef struct {
    usize ra;
    usize sp;
    usize s[12];
} TaskContext;

#endif /* __TASK_CONTEXT_H_ */
