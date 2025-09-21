#ifndef INTERRUPT_H
#define INTERRUPT_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

/**
 * @brief Interrupt operations structure.
 */
struct interrupt_ops
{
    /**
     * @brief Enables global interrupts.
     */
    void (*enable_global)(bool);
};

/* ============================================================================================== */

struct interrupt
{
    const struct interrupt_ops* ops;
};

/* ============================================================================================== */

#endif
