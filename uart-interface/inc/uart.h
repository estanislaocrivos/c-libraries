#ifndef UART_H
#define UART_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the UART receive callback function.
 * @param buffer Pointer to the received data buffer.
 * @param length Length of the received data.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 */
typedef void (*uart_rx_callback_t)(const uint8_t* buffer,
                                   size_t         buffer_size,
                                   void*          callback_context);

/* ============================================================================================== */

struct uart_port;  // Forward declaration. Declared later.

/* ============================================================================================== */

struct uart_ops
{
    /**
     * @brief Initializes the UART interface.
     * @param p Pointer to the UART port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct uart_port* port);

    /**
     * @brief Transmits data through the UART interface.
     * @param port Pointer to the UART port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(struct uart_port* port, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the UART interface.
     * @param port Pointer to the UART port structure.
     * @param buffer Pointer to the 8-bit buffer where the received data will be stored.
     * @param size Size of the buffer to be received.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(struct uart_port* port, uint8_t* buffer, size_t size);

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param port Pointer to the UART port structure.
     * @param callback The callback function to set. Its prototype must match the uart_rx_callback_t
     * type.
     * @param callback_context Pointer to user-defined callback_context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(struct uart_port*  port,
                              uart_rx_callback_t callback,
                              void*              callback_context);

    /**
     * @brief Enables or disables the UART receive interrupt.
     * @param port Pointer to the UART port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     */
    void (*enable_rx_interrupt)(struct uart_port* port, bool enable);

    /**
     * @brief Clears the UART buffers.
     * @param port Pointer to the UART port structure.
     */
    void (*clear_buffers)(struct uart_port* p);
};

/* ============================================================================================== */

struct uart_port
{
    uint32_t baud_rate;
    uint8_t* rx_buffer;
    size_t   rx_buffer_size;
    void*    callback_context;

    /**
     * @brief Pointer to the UART operations structure.
     */
    const struct uart_ops* ops;
};

/* ============================================================================================== */

/*
Usage:
int main(void)
{
    const struct uart_ops my_uart_ops = {
        .uart_initialize_t = my_uart_initialize,
        .uart_transmit_t = my_uart_transmit,
        .uart_receive_t = my_uart_receive,
        .uart_set_rx_callback_t = my_uart_set_rx_callback,
        .uart_enable_rx_interrupt_t = my_uart_enable_rx_interrupt,
        .uart_clear_buffers_t = my_uart_clear_buffers,
    };
    struct uart_port my_uart;
    my_uart.ops = &my_uart_ops;
    my_uart.baud_rate = 9600;
    my_uart.rx_buffer = malloc(128);
    my_uart.rx_buffer_size = 128;
    my_uart.ops->uart_initialize_t(&my_uart);
    // Other application code...
    return 0;
}
*/

/* ============================================================================================== */

#endif  // UART_H
