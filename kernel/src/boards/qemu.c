/**
 * @file qemu.c
 * @brief Constants used in cCore for qemu.
 */

#include "qemu.h"

const MMIORange MMIO[] = {
    { 0x00100000, 0x00002000 },  // VIRT_TEST/RTC virt machine
};

const usize MMIO_LEN = sizeof(MMIO) / sizeof(MMIO[0]);
