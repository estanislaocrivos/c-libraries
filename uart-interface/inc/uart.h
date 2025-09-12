#ifndef UART_H
#define UART_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../inc/errno.h"

/* ============================================================================================== */

/* RX callback prototype. Receives a pointer to the received buffer and its length */
typedef void (*uart_rx_callback_t)(const uint8_t* buffer, size_t length);

/* ============================================================================================== */

typedef int8_t (*uart_initialize_t)(void);
typedef int8_t (*uart_transmit_t)(const uint8_t* tx_buffer, size_t length);
typedef int8_t (*uart_receive_t)(uint8_t* rx_buffer, size_t length);
typedef int8_t (*uart_set_rx_callback_t)(uart_rx_callback_t callback);
typedef void (*uart_clear_buffers_t)(void);

/* ============================================================================================== */

typedef struct
{
    /**
     * @brief Initializes the UART interface.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    uart_initialize_t initialize;

    /**
     * @brief Transmits data through the UART interface.
     * @param tx_buffer Pointer to the 8-bit buffer to be transmitted.
     * @param length Length of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    uart_transmit_t transmit;

    /**
     * @brief Receives data through the UART interface.
     * @param rx_buffer Pointer to the 8-bit buffer where the received data will be stored.
     * @param length Length of the buffer to be received.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    uart_receive_t receive;

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param callback The callback function to set. Its prototype must match the uart_rx_callback_t
     * type.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    uart_set_rx_callback_t set_rx_callback;

    /**
     * @brief Clears the UART buffers.
     */
    uart_clear_buffers_t clear_buffers;
} uart_t;

/* ============================================================================================== */

/*
Usage:

int main(void)
{
    const uart_t uart = {uart1_initialize, uart1_transmit, uart1_receive, uart1_set_rx_callback,
                         uart1_clear_buffers};
    uart.initialize();
    return 0;
}
*/

/* ============================================================================================== */

#endif  // UART_H
