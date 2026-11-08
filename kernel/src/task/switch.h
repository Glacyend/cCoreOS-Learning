#ifndef __TASK_SWITCH_H_
#define __TASK_SWITCH_H_

#include "context.h"

/**
 * @brief Switch from @p current to @p next .
 *
 * Saves callee-saved registers of the current task into @p current , then
 * restores the callee-saved registers of @p next and jumps to its saved `ra`.
 *
 * @param current Context to save the current task into.
 * @param next    Context to restore.
 */
void __switch(TaskContext* current, TaskContext* next);

#endif /* __TASK_SWITCH_H_ */
