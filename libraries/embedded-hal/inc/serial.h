#ifndef SERIAL_H
#define SERIAL_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the serial receive callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 * @param byte The received byte.
 */
typedef void (*serial_rx_callback_t)(void* callback_context, uint8_t byte);

/* ============================================================================================== */

/**
 * @brief serial port structure. This structure holds the configuration and state of a serial port.
 */
struct serial_port;

/* ============================================================================================== */

/**
 * @brief serial operations structure. This structure holds function pointers for serial operations.
 */
struct serial_ops
{
    /**
     * @brief Initializes the serial interface.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(const struct serial_port* self);

    /**
     * @brief Transmits data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(const struct serial_port* self, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param data Pointer to the data byte where the received data will be stored.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(const struct serial_port* self, uint8_t* data);

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param self Pointer to the serial port structure.
     * @param callback The callback function to set. Its prototype must match the
     * serial_rx_callback_t type.
     * @param callback_context Pointer to user-defined callback_context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(const struct serial_port* self,
                              serial_rx_callback_t      callback,
                              void*                     callback_context);

    /**
     * @brief Enables or disables the serial receive interrupt.
     * @param self Pointer to the serial port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*enable_rx_interrupt)(const struct serial_port* self, bool enable);

    /**
     * @brief Flushes the transmit buffer.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_tx)(const struct serial_port* self);

    /**
     * @brief Flushes the receive buffer.
     * @param self Pointer to the serial port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_rx)(const struct serial_port* self);
};

/* ============================================================================================== */

struct serial_port
{
    /**
     * @brief serial port identifier. Serves to identify different serial peripherals within the
     * same driver.
     */
    uint8_t port_id;

    /**
     * @brief Baud rate for serial communication.
     */
    uint32_t baud_rate;

    /**
     * @brief Number of data bits (5, 6, 7, 8, or 9).
     */
    uint8_t data_bits;

    /**
     * @brief Parity configuration: 'N' (None), 'E' (Even), 'O' (Odd), 'M' (Mark), 'S' (Space).
     */
    char parity;

    /**
     * @brief Number of stop bits: 1 or 2 (some support 0.5 or 1.5).
     */
    uint8_t stop_bits;

    bool  _was_initialized;   // Internal flag to prevent reinitialization or misuse.
    void* _callback_context;  // Pointer to user-defined callback_context data (can be NULL).

    /**
     * @brief Pointer to the serial operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct serial_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct serial_ops* ops;
};

/* ============================================================================================== */

#endif  // SERIAL_H
