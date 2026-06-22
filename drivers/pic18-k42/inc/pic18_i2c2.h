/**
 * @file
 * @brief PIC18-K42 driver header file.
 */

/* ========================================================================== */

#ifndef PIC18_I2C2_H
#define PIC18_I2C2_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "i2c.h"
#include "pic18_k42.h"

/* ========================================================================== */

extern const struct i2c_ops pic18_i2c2_ops;

/* ========================================================================== */

/**
 * @brief Initializes the I2C2 interface.
 * @param self Pointer to the I2C port structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_i2c2_initialize(struct i2c* self);

/* ========================================================================== */

/**
 * @brief Transmits data through the I2C2 interface.
 * @param self Pointer to the I2C port structure.
 * @param reg_address Register address to write to.
 * @param tx_payload Pointer to the data to be transmitted.
 * @param tx_payload_size Size of the data to be transmitted in bytes.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 * @note This function performs a write to the register address followed by the
 * data, all in the same transaction without a stop condition in between.
 */
int8_t pic18_i2c2_write(
    const struct i2c* self,
    uint8_t           reg_address,
    const uint8_t*    tx_payload,
    size_t            tx_payload_size);

/* ========================================================================== */

/**
 * @brief Receives data through the I2C2 interface.
 * @param self Pointer to the I2C port structure.
 * @param reg_address Register address to read from.
 * @param rx_payload Pointer to the buffer where received data will be stored.
 * @param rx_payload_size Size of the data to be received in bytes.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 * @note This function performs a write to the register address followed by a
 * read with a repeated start condition.
 */
int8_t pic18_i2c2_read(
    const struct i2c* self,
    uint8_t           reg_address,
    uint8_t*          rx_payload,
    size_t            rx_payload_size);

/* ========================================================================== */

/**
 * @brief Transmits raw data through the I2C2 interface without register
 * addressing.
 * @param self Pointer to the I2C port structure.
 * @param tx_payload Pointer to the data to be transmitted.
 * @param tx_payload_size Size of the data to be transmitted in bytes.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 * @note Performs a simple write transaction: START + ADDR(W) + DATA + STOP.
 */
int8_t pic18_i2c2_write_raw(
    const struct i2c* self, const uint8_t* tx_payload, size_t tx_payload_size);

/* ========================================================================== */

/**
 * @brief Receives raw data through the I2C2 interface without register
 * addressing.
 * @param self Pointer to the I2C port structure.
 * @param rx_payload Pointer to the buffer where received data will be stored.
 * @param rx_payload_size Size of the data to be received in bytes.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 * @note Performs a simple read transaction: START + ADDR(R) + DATA + STOP.
 */
int8_t pic18_i2c2_read_raw(
    const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size);

/* ========================================================================== */

/**
 * @brief Probes the I2C bus to check if a device is present.
 * @param self Pointer to the I2C port structure.
 * @return int8_t Returns 0 if device ACKs, -ENXIO if no ACK, or -ERR on
 * failure.
 * @note Sends START + ADDR(W) + STOP and checks for ACK. Uses the slave_address
 * configured in the i2c structure.
 */
int8_t pic18_i2c2_probe(const struct i2c* self);

/* ========================================================================== */

#endif /* PIC18_I2C2_H */
