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

    /**
     * @brief Makes a code section atomic by disabling interrupts. Must allow multiple nested calls
     * with internal counter.
     */
    void (*atomic_lock)(void);

    /**
     * @brief Restores the previous interrupt state. Must match a previous call to atomic_lock().
     */
    void (*atomic_unlock)(void);
};

/* ============================================================================================== */

struct interrupt
{
    const struct interrupt_ops* ops;
};

/* ============================================================================================== */

#endif
