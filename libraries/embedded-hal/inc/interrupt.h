#ifndef INTERRUPT_H
#define INTERRUPT_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

#define INTERRUPT_DEFINE(name, ops_ptr) \
    struct interrupt name = {.ops = (ops_ptr)}

/* ========================================================================== */

/**
 * @brief Interrupt operations structure.
 */
struct interrupt_ops
{
    /**
     * @brief Enables global interrupts.
     */
    void (*enable_global)(bool);

    /**
     * @brief Enables low priority interrupts.
     */
    void (*enable_low_priority)(bool);

    /**
     * @brief Enables high priority interrupts.
     */
    void (*enable_high_priority)(bool);

    /**
     * @brief Makes a code section atomic by disabling interrupts. Must allow
     * multiple nested calls with internal counter.
     */
    void (*atomic_lock)(void);

    /**
     * @brief Restores the previous interrupt state. Must match a previous call
     * to atomic_lock().
     */
    void (*atomic_unlock)(void);
};

/* ========================================================================== */

struct interrupt
{
    const struct interrupt_ops* const ops;
};

/* ========================================================================== */

#endif
