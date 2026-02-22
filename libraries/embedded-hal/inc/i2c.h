#ifndef I2C_H
#define I2C_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#define I2C_DEFINE(                                                  \
    name, ops_ptr, is_master, freq, addr_mode, slave_addr, own_addr) \
    struct i2c name                                                  \
        = {.ops             = (ops_ptr),                             \
           .master          = (is_master),                           \
           .frequency       = (freq),                                \
           .address_mode    = (addr_mode),                           \
           .slave_address   = (slave_addr),                          \
           .own_address     = (own_addr),                            \
           .was_initialized = false}

/* ========================================================================== */

/**
 * @brief I2C interface structure.
 */
struct i2c;

/**
 * @brief I2C addressing modes.
 */
enum i2c_address_mode
{
    I2C_ADDR_7BIT  = 0,
    I2C_ADDR_10BIT = 1,
};

/**
 * @brief I2C operations structure. This structure holds function pointers for
 * I2C operations.
 */
struct i2c_ops
{
    /**
     * @brief Initializes the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct i2c* self);

    /**
     * @brief Transmits data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param reg_address Register address to write to.
     * @param tx_payload Pointer to the data to be transmitted.
     * @param tx_payload_size Size of the data to be transmitted in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address
     * pointer register followed by a write operation, all in the same
     * transaction without a stop condition in between.
     */
    int8_t (*write)(
        const struct i2c* self,
        uint8_t           reg_address,
        const uint8_t*    tx_payload,
        size_t            tx_payload_size);

    /**
     * @brief Receives data through the I2C interface.
     * @param self Pointer to the I2C port structure.
     * @param reg_address Register address to read from.
     * @param rx_payload Pointer to the buffer where received data will be
     * stored.
     * @param rx_payload_size Size of the data to be received in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note In master mode, this function performs a write to the reg. address
     * pointer register followed by a read operation with a repeated start
     * condition.
     */
    int8_t (*read)(
        const struct i2c* self,
        uint8_t           reg_address,
        uint8_t*          rx_payload,
        size_t            rx_payload_size);

    /**
     * @brief Transmits raw data through the I2C interface without register
     * addressing.
     * @param self Pointer to the I2C port structure.
     * @param tx_payload Pointer to the data to be transmitted.
     * @param tx_payload_size Size of the data to be transmitted in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note Performs a simple write transaction: START + ADDR(W) + DATA + STOP.
     */
    int8_t (*write_raw)(
        const struct i2c* self,
        const uint8_t*    tx_payload,
        size_t            tx_payload_size);

    /**
     * @brief Receives raw data through the I2C interface without register
     * addressing.
     * @param self Pointer to the I2C port structure.
     * @param rx_payload Pointer to the buffer where received data will be
     * stored.
     * @param rx_payload_size Size of the data to be received in bytes.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note Performs a simple read transaction: START + ADDR(R) + DATA + STOP.
     */
    int8_t (*read_raw)(
        const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size);

    /**
     * @brief Probes the I2C bus to check if a device is present.
     * @param self Pointer to the I2C port structure.
     * @return int8_t Returns 0 if device ACKs, -ENXIO if no ACK, or -ERR on
     * failure.
     * @note Sends START + ADDR(W) + STOP and checks for ACK. Uses the
     * slave_address configured in the i2c structure.
     */
    int8_t (*probe)(const struct i2c* self);
};

/* ========================================================================== */

struct i2c
{
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

    /**
     * @brief Pointer to the I2C operations structure.
     */
    const struct i2c_ops* const ops;

    /* private: internal state - do not access directly */
    bool was_initialized;
};

/* ========================================================================== */

#endif  // I2C_H
