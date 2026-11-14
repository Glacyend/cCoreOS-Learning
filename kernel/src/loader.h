/**
 * @file loader.h
 * @brief Loading user applications into memory.
 */

#ifndef __LOADER_H_
#define __LOADER_H_

<<<<<<< Updated upstream
#include "config.h"
=======
>>>>>>> Stashed changes
#include "mm/slice.h"

/**
 * @brief Get the total number of applications.
 *
 * Rust equivalent: `loader::get_num_app`.
 */
size_t loader_get_num_app();

/**
 * @brief Get applications data.
 *
 * Rust equivalent: `loader::get_app_data`. Returns the ELF bytes of app
 * @p app_id as a slice into the kernel image.
 *
 * @param app_id App index, 0-based.
 */
<<<<<<< Updated upstream
ByteSlice loader_get_app_data(usize app_id);
=======
ByteSlice loader_get_app_data(size_t app_id);
>>>>>>> Stashed changes

#endif /* __LOADER_H_ */
