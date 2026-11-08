/**
 * @file loader.h
 * @brief Load user applications embedded in the kernel image.
 *
 * Every app is a raw `.bin` embedded via `.incbin`. The loader copies app @c i
 * to @c APP_BASE_ADDRESS+i*APP_SIZE_LIMIT before running it. All apps share
 * the same virtual address so only one runs at a time (batch scheduling).
 */

#ifndef __LOADER_H_
#define __LOADER_H_

#include "config.h"

/**
 * @brief Number of apps embedded in the kernel.
 *
 * @return Number of apps.
 */
usize loader_get_num_app();

/**
 * @brief Copy app @p app_id into its slot.
 *
 * Zeroes the whole slot, then copies the embedded bytes. Executes `fence.i` so
 * the instruction caches sees the new code.
 *
 * @param app_id App index, 0-based.
 */
void loader_load_app(usize app_id);

/**
 * @brief Copy every embedded app into its slot.
 *
 * Must be called once before any task runs.
 */
void loader_load_apps();

/**
 * @brief Initialize a `TrapContext` for app @p app_id and push it onto that
 *        app's kernel stack.
 *
 * The entry point is `APP_BASE_ADDRESS + app_id * APP_SIZE_LIMIT`; the user
 * stack comes from the tail of that app's slot.
 *
 * @param app_id App index, 0-based.
 * @return Address of the `TrapContext` on the kernel stack, suitable as the
 *         `sp` for a `TaskContext` whose `ra` is `__restore`.
 */
usize loader_init_app_cx(usize app_id);

#endif /* __LOADER_H_ */
