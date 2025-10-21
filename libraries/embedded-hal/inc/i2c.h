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
     * @param reg_address Register address to write to.
     * @param tx_payload Pointer to the data to be transmitted.
     * @param tx_payload_size Size of the data to be transmitted in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address pointer register
     * followed by a write operation, all in the same transaction without a stop condition in
     * between.
     */
    int8_t (*write)(struct i2c_port* self,
                    uint8_t          reg_address,
                    const uint8_t*   tx_payload,
                    size_t           tx_payload_size);

    /**
     * @brief Receives data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param reg_address Register address to read from.
     * @param rx_payload Pointer to the buffer where received data will be stored.
     * @param rx_payload_size Size of the data to be received in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address pointer register
     * followed by a read operation with a repeated start condition.
     */
    int8_t (*read)(struct i2c_port* self,
                   uint8_t          reg_address,
                   uint8_t*         rx_payload,
                   size_t           rx_payload_size);
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

    bool _was_initialized;  // Internal flag to prevent reinitialization or misuse.

    /**
     * @brief Pointer to the I2C operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct i2c_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct i2c_ops* ops;
};

/* ============================================================================================== */

#endif  // I2C_H
