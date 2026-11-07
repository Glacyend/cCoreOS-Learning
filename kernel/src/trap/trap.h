/**
 * @file trap.h
 * @brief Trap handling.
 */

#ifndef __TRAP_H_
#define __TRAP_H_

#include "context.h"

/**
 * @brief Set `stvec` to `__alltraps`.
 *
 * Must be called before any trap can occur.
 */
void trap_init();

/**
 * @brief Dispatch a trap.
 *
 * Called from `__alltraps` with `a0 = &TrapContext`.
 *
 * @param cx Pointer to the saved context.
 * @return The same `cx` pointer, so that `__restore` can pick it up from `a0`.
 */
TrapContext* trap_handler(TrapContext* cx);

#endif /* __TRAP_H_ */
