/**
 * @file
 * @brief PIC18-K42 interrupt driver header file.
 */

/* ========================================================================== */

#ifndef PIC18_INTERRUPT_H
#define PIC18_INTERRUPT_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "interrupt.h"
#include "pic18_k42.h"

/* ========================================================================== */

/**
 * @brief PIC18 interrupt operations structure.
 *
 * This structure provides the implementation of interrupt operations for PIC18
 * microcontrollers. Use this to initialize a struct interrupt instance.
 *
 * Example usage:
 * @code
 * struct interrupt irq = {
 *     .ops = &pic18_interrupt_ops,
 * };
 * @endcode
 */
extern const struct interrupt_ops pic18_interrupt_ops;

/* ========================================================================== */

/**
 * @brief Enables or disables global interrupts.
 * @param enable true to enable global interrupts, false to disable.
 */
void pic18_global_interrupt_enable(bool enable);

/**
 * @brief Enters an atomic (critical) section by disabling interrupts.
 *
 * Supports nested calls with an internal counter. Each call to atomic_lock()
 * must be matched with a corresponding call to atomic_unlock().
 */
void pic18_interrupt_atomic_lock(void);

/**
 * @brief Exits an atomic (critical) section.
 *
 * Restores the previous interrupt state when all nested locks have been
 * released. Must match a previous call to atomic_lock().
 */
void pic18_interrupt_atomic_unlock(void);

/* ========================================================================== */

/**
 * @brief Enables or disables low priority interrupts.
 * @param enable true to enable low priority interrupts, false to disable.
 */
void pic18_low_priority_interrupt_enable(bool enable);

/* ========================================================================== */

/**
 * @brief Enables or disables high priority interrupts.
 * @param enable true to enable high priority interrupts, false to disable.
 */
void pic18_high_priority_interrupt_enable(bool enable);

/* ========================================================================== */

/**
 * @brief ISR callback type.
 */
typedef void (*pic18_isr_callback_t)(void);

/**
 * @brief Interrupt source identifiers.
 */
typedef enum
{
    PIC18_IRQ_UART1_RX,
    PIC18_IRQ_UART2_RX,
    PIC18_IRQ_ADCC_CONVERSION,
    PIC18_IRQ_SPI1_RX,
    PIC18_IRQ_SPI1_TX,
    PIC18_IRQ_TMR0,
    PIC18_IRQ_TMR2,
    PIC18_IRQ_TMR4,
    PIC18_IRQ_TMR6,
    PIC18_IRQ_CCP1,
    PIC18_IRQ_CCP2,
    PIC18_IRQ_CCP3,
    PIC18_IRQ_TMR1,
    PIC18_IRQ_TMR3,
    PIC18_IRQ_TMR5,
    PIC18_IRQ_COUNT
} pic18_irq_source_t;

/**
 * @brief Registers an ISR callback for a specific interrupt source.
 * @param source The interrupt source identifier.
 * @param callback The callback function to register (NULL to unregister).
 * @return 0 on success, -EINVAL if source is invalid.
 */
int8_t pic18_isr_register(
    pic18_irq_source_t source, pic18_isr_callback_t callback);

/* ========================================================================== */

#endif
