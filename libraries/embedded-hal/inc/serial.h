#ifndef serial_H
#define serial_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the serial receive callback function.
 * @param buffer Pointer to the received data buffer.
 * @param length Length of the received data.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 */
typedef void (*serial_rx_callback_t)(void*          callback_context,
                                     const uint8_t* buffer,
                                     size_t         buffer_size);

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
    int8_t (*initialize)(struct serial_port* self);

    /**
     * @brief Transmits data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(struct serial_port* self, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the serial interface.
     * @param self Pointer to the serial port structure.
     * @param buffer Pointer to the 8-bit buffer where the received data will be stored.
     * @param size Size of the buffer to be received.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(struct serial_port* self, uint8_t* buffer, size_t size);

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param self Pointer to the serial port structure.
     * @param callback The callback function to set. Its prototype must match the
     * serial_rx_callback_t type.
     * @param callback_context Pointer to user-defined callback_context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(struct serial_port*  self,
                              serial_rx_callback_t callback,
                              void*                callback_context);

    /**
     * @brief Enables or disables the serial receive interrupt.
     * @param self Pointer to the serial port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     */
    void (*enable_rx_interrupt)(struct serial_port* self, bool enable);

    /**
     * @brief Clears the serial buffers.
     * @param self Pointer to the serial port structure.
     */
    void (*clear_buffers)(struct serial_port* self);
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
     * @brief Pointer to the receive buffer. This buffer must be created by the user, assigned, and
     * kept alive throughout the lifetime of the serial port.
     */
    uint8_t* rx_buffer;

    /**
     * @brief Size of the receive buffer in bytes. This size must match the actual size of the
     * buffer created by the user.
     */
    size_t rx_buffer_size;

    /**
     * @brief Pointer to the user-defined callback_context data (can be NULL).
     */
    void* callback_context;

    /**
     * @brief Pointer to the serial operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct serial_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct serial_ops* ops;
};

/* ============================================================================================== */

/*
Usage:
int main(void)
{
    const struct serial_ops my_serial_ops = {
        .initialize = my_serial_initialize,
        .transmit = my_serial_transmit,
        .receive = my_serial_receive,
        .set_rx_callback = my_serial_set_rx_callback,
        .enable_rx_interrupt = my_serial_enable_rx_interrupt,
        .clear_buffers = my_serial_clear_buffers,
    };
    struct serial_port my_serial;
    my_serial.ops = &my_serial_ops;
    my_serial.baud_rate = 9600;
    my_serial.rx_buffer = malloc(128);
    my_serial.rx_buffer_size = 128;
    my_serial.ops->initialize(&my_serial);
    // Other application code...
    return 0;
}
*/

/* ============================================================================================== */

#endif  // serial_H
