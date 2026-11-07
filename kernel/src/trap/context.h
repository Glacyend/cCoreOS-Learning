/**
 * @file context.h
 * @brief Trap context: register snapshot saved on trap entry.
 *
 * Layout matches `trap.S` exactly:
 *
 *   offset    field
 *   0*8       x[0] (never written; always 0)
 *   1*8       x[1] ra
 *   2*8       x[2] sp (user stack pointer)
 *   3*8       x[3] gp
 *   4*8       x[4] tp (never written; user programs do not use it)
 *   5*8..31*8 x[5]..x[31]
 *   32*8      sstatus
 *   33*8      spec
 *
 * Total size: 34*8 = 272 bytes.
 */

#ifndef __TRAP_CONTEXT_H_
#define __TRAP_CONTEXT_H_

#include "../config.h"

typedef struct {
    usize x[32];
    usize sstatus;
    usize sepc;
} TrapContext;

#endif /* __TRAP_CONTEXT_H_ */
