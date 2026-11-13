/**
 * @file loader.h
 * @brief Loading user applications into memory.
 */

#ifndef __LOADER_H_
#define __LOADER_H_

#include "config.h"
#include "mm/slice.h"

/**
 * @brief Get the total number of applications.
 *
 * Rust equivalent: `loader::get_num_app`.
 */
usize loader_get_num_app();

/**
 * @brief Get applications data.
 *
 * Rust equivalent: `loader::get_app_data`. Returns the ELF bytes of app
 * @p app_id as a slice into the kernel image.
 *
 * @param app_id App index, 0-based.
 */
ByteSlice loader_get_app_data(usize app_id);

#endif /* __LOADER_H_ */
