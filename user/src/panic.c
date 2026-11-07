/**
 * @file panic.c
 * @brief User-space panic handler.
 */

#include "console.h"
#include "lib.h"

#include <stdarg.h>

void user_panic(const char* msg) {
    printf("panic: %s\n", msg);
    exit(1);
}
