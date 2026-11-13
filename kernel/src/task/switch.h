/**
 * @file switch.h
 * @brief C wrapper around `__switch`.
 *
 * Switching to a different task's context happens here. The actual
 * implementation must not be in C and (essentially) has to be in assembly
 * language, so this module is really just a wrapper around `switch.S`.
 */

#ifndef __TASK_SWITCH_H_
#define __TASK_SWITCH_H_

#include "context.h"

/**
 * @brief Switch to the context of @p next_task_cx_ptr , saving the current
 *        context in @p current_task_cx_ptr .
 *
 * Rust equivalent: `__switch`. The implementation lives in `switch.S`, which
 * is compiled as a separate translation unit by `kernel/CMakeLists.txt`.
 *
 * The caller must ensure:
 *   - both pointers point to valid `TaskContext` objects;
 *   - @p current_task_cx_ptr is writable and @p next_task_cx_ptr is readable;
 *   - the two pointers do not alias.
 *   - both contexts remain alive across the call, because control flow leaves
 *     the caller and does not return until another task switches back.
 *
 * @param current_task_cx_ptr Context to save the current task into.
 * @param next_task_cx_ptr    Context to restore.
 */
extern void __switch(TaskContext* current_task_cx_ptr, const TaskContext* next_task_cx_ptr);

#endif /* __TASK_SWITCH_H_ */
