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

#endif /* __LOADER_H_ */