#ifndef I2C_H
#define I2C_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the I2C receive callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 * @param byte The received byte.
 */
typedef void (*i2c_rx_callback_t)(void* callback_context, uint8_t byte);

/* ============================================================================================== */

/**
 * @brief I2C port structure. This structure holds the configuration and state of a I2C port.
 */
struct i2c_port;

/**
 * @brief I2C addressing modes.
 */
enum i2c_address_mode
{
    I2C_ADDR_7BIT  = 0,
    I2C_ADDR_10BIT = 1,
};

/**
 * @brief I2C operations structure. This structure holds function pointers for I2C operations.
 */
struct i2c_ops
{
    /**
     * @brief Initializes the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct i2c_port* self);

    /**
     * @brief Transmits data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(struct i2c_port* self, const uint8_t byte);

    /**
     * @brief Receives data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param buffer Pointer to the byte where the received data will be stored.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(struct i2c_port* self, uint8_t* byte);

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param self Pointer to the I2C port structure.
     * @param callback The callback function to set. Its prototype must match the i2c_rx_callback_t
     * type.
     * @param callback_context Pointer to user-defined callback_context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(struct i2c_port*  self,
                              i2c_rx_callback_t callback,
                              void*             callback_context);

    /**
     * @brief Enables or disables the I2C receive interrupt.
     * @param self Pointer to the I2C port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     */
    int8_t (*enable_rx_interrupt)(struct i2c_port* self, bool enable);

    /**
     * @brief Clears the I2C buffers.
     * @param self Pointer to the I2C port structure.
     */
    int8_t (*flush_buffer)(struct i2c_port* self);
};

/* ============================================================================================== */

struct i2c_port
{
    /**
     * @brief I2C port identifier (e.g., 0 for i2c1, 1 for i2c2, etc.).
     */
    uint8_t port_id;

    /**
     * @brief Master or Slave mode. true for Master, false for Slave.
     */
    bool master;

    /**
     * @brief Clock frequency in Hertz.
     */
    uint32_t frequency;

    /**
     * @brief Addressing mode (7-bit or 10-bit).
     */
    enum i2c_address_mode address_mode;

    /**
     * @brief I2C slave address (7-bit or 10-bit). Only used in master mode.
     */
    uint16_t slave_address;

    /**
     * @brief Own address when in slave mode.
     */
    uint16_t own_address;

    bool  _was_initialized;   // Internal flag to prevent reinitialization or misuse.
    void* _callback_context;  // Pointer to user-defined callback_context data (can be NULL).

    /**
     * @brief Pointer to the I2C operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct i2c_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct i2c_ops* ops;
};

/* ============================================================================================== */

#endif  // I2C_H
