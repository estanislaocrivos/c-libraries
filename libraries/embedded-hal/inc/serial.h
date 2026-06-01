#ifndef SERIAL_H
#define SERIAL_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

#define SERIAL_DEFINE(name, ops_ptr, baud, dbits, par, sbits) \
    struct serial name                                        \
        = {.ops             = (ops_ptr),                      \
           .baud_rate       = (baud),                         \
           .data_bits       = (dbits),                        \
           .parity          = (par),                          \
           .stop_bits       = (sbits),                        \
           .was_initialized = false}

/* ========================================================================== */

/**
 * @brief Prototype for the serial receive callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be
 * NULL).
 * @param byte The received byte.
 */
typedef void (*serial_rx_callback_t)(void* callback_context, uint8_t byte);

/* ========================================================================== */

/**
 * @brief Serial interface structure.
 */
struct serial;

/* ========================================================================== */

/**
 * @brief serial operations structure. This structure holds function pointers
 * for serial operations.
 */
struct serial_ops
{
    /**
     * @brief Initializes the serial interface.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct serial* self);

    /**
     * @brief Transmits data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(
        const struct serial* self, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param data Pointer to the data byte where the received data will be
     * stored.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(const struct serial* self, uint8_t* data);

    /**
     * @brief Sets the callback function to be called when a full word has been
     * received.
     * @param self Pointer to the serial port structure.
     * @param callback The callback function to set. Its prototype must match
     * the serial_rx_callback_t type.
     * @param callback_context Pointer to user-defined callback_context data
     * (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(
        const struct serial* self,
        serial_rx_callback_t callback,
        void*                callback_context);

    /**
     * @brief Enables or disables the serial receive interrupt.
     * @param self Pointer to the serial port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*enable_rx_interrupt)(const struct serial* self, bool enable);

    /**
     * @brief Sets the priority of the serial interrupt.
     * @param self Pointer to the serial port structure.
     * @param priority The priority level to set (0 = highest priority).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_interrupt_priority)(
        const struct serial* self, uint8_t priority);

    /**
     * @brief Flushes the transmit buffer.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_tx)(const struct serial* self);

    /**
     * @brief Flushes the receive buffer.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_rx)(const struct serial* self);
};

/* ========================================================================== */

struct serial
{
    /**
     * @brief Baud rate for serial communication.
     */
    uint32_t baud_rate;

    /**
     * @brief Number of data bits (5, 6, 7, 8, or 9).
     */
    uint8_t data_bits;

    /**
     * @brief Parity configuration: 'N' (None), 'E' (Even), 'O' (Odd), 'M'
     * (Mark), 'S' (Space).
     */
    char parity;

    /**
     * @brief Number of stop bits: 1 or 2 (some support 0.5 or 1.5).
     */
    uint8_t stop_bits;

    /**
     * @brief Pointer to the serial operations structure.
     */
    const struct serial_ops* const ops;

    /* private: internal state - do not access directly */
    bool  was_initialized;
    void* callback_context;
};

/* ========================================================================== */

#endif  // SERIAL_H
