/**
 * @file
 * @brief PIC18-K42 UART1 driver header file.
 */

/* ========================================================================== */

#ifndef PIC18_UART1_H
#define PIC18_UART1_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "pic18_k42.h"
#include "serial.h"

/* ========================================================================== */

extern const struct serial_ops pic18_uart1_ops;

/* ========================================================================== */

/**
 * @brief Initializes the serial interface.
 * @param self Pointer to the serial port structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_initialize(struct serial* self);

/* ========================================================================== */

/**
 * @brief Transmits data through the serial interface.
 * @param self Pointer to the serial port structure.
 * @param buffer Pointer to the 8-bit buffer to be transmitted.
 * @param size Size of the buffer to be transmitted.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size);

/* ========================================================================== */

/**
 * @brief Receives data through the serial interface.
 * @param self Pointer to the serial port structure.
 * @param data Pointer to the data byte where the received data will be stored.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_receive(const struct serial* self, uint8_t* data);

/* ========================================================================== */

/**
 * @brief Sets the callback function to be called when a full word has been
 * received.
 * @param self Pointer to the serial port structure.
 * @param callback The callback function to set. Its prototype must match the
 * serial_rx_callback_t type.
 * @param callback_context Pointer to user-defined callback_context data (can be
 * NULL).
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context);

/* ========================================================================== */

/**
 * @brief Enables or disables the serial receive interrupt.
 * @param self Pointer to the serial port structure.
 * @param enable Set to true to enable the interrupt, false to disable it.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_enable_rx_interrupt(const struct serial* self, bool enable);

/* ========================================================================== */

/**
 * @brief Sets the priority of the serial interrupt.
 * @param self Pointer to the serial port structure.
 * @param priority The priority level to set (0 = highest priority).
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_uart1_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority);

/* ========================================================================== */

/**
 * @brief Flushes the transmit buffer.
 * @param self Pointer to the serial port structure.
 * @return int8_t Returns 0 on success or -ERR on failure.
 */
int8_t pic18_uart1_flush_tx(const struct serial* self);

/* ========================================================================== */

/**
 * @brief Flushes the receive buffer.
 * @param self Pointer to the serial port structure.
 * @return int8_t Returns 0 on success or -ERR on failure.
 */
int8_t pic18_uart1_flush_rx(const struct serial* self);

/* ========================================================================== */

#endif /* PIC18_UART1_H */
