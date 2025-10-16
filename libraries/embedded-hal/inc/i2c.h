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
 * @brief I2C transfer structure. This structure holds the details of an I2C transfer.
 */
struct i2c_transfer
{
    /**
     * @brief Pointer to the register address to read from or write to.
     */
    const uint8_t* reg_address;

    /**
     * @brief Size of the register address in bytes (1 or 2).
     */
    size_t reg_address_size;

    /**
     * @brief Pointer to the data to be written.
     */
    const uint8_t* tx_payload;

    /**
     * @brief Size of the data to be written in bytes.
     */
    size_t tx_payload_size;

    /**
     * @brief Pointer to the buffer where the received data will be stored.
     */
    uint8_t* rx_payload;

    /**
     * @brief Size of the buffer to be received in bytes.
     */
    size_t rx_payload_size;
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
     * @param transfer Pointer to the i2c_transfer structure containing transfer details. Must
     * contain reg_address, reg_address_size, tx_payload, tx_payload_size.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address pointer register
     * followed by a write operation, all in the same transaction without a stop condition in
     * between.
     */
    int8_t (*write)(struct i2c_port* self, const struct i2c_transfer* transfer);

    /**
     * @brief Receives data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param transfer Pointer to the i2c_transfer structure containing transfer details. Must
     * contain reg_address, reg_address_size, rx_payload, rx_payload_size.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address pointer register
     * followed by a read operation with a repeated start condition.
     */
    int8_t (*read)(struct i2c_port* self, struct i2c_transfer* transfer);

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
