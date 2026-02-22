#ifndef CRC_H
#define CRC_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * struct crc - CRC-8 calculator configuration
 * @crc8_polynomial: CRC-8 polynomial
 * @crc8_initial_value: Initial CRC value
 * @crc8_final_xor_value: Final XOR value applied to result
 * @reflect_input: Reflect input bytes before processing
 * @reflect_output: Reflect output CRC before final XOR
 *
 * Configuration for CRC-8 calculation. All fields are public and must be
 * set by the user before calling crc8_calculate(). Supports standard
 * configurations like AUTOSAR, Bluetooth, CDMA2000, DARC, and others.
 */
struct crc
{
    /* public: all fields are user-configurable (const - configuration only) */
    const uint8_t crc8_polynomial;
    const uint8_t crc8_initial_value;
    const uint8_t crc8_final_xor_value;
    const bool    reflect_input;
    const bool    reflect_output;
};

/* ========================================================================== */

/**
 * @brief Calculate CRC-8 checksum.
 * @param crc Pointer to the CRC configuration.
 * @param data Pointer to input data.
 * @param length Length of input data in bytes.
 * @param result Pointer to store the calculated CRC-8 value.
 * @return 0 on success, -EFAULT if any pointer is NULL, -EINVAL if length is 0.
 */
int8_t crc8_calculate(
    const struct crc* crc, const uint8_t* data, size_t length, uint8_t* result);

/* ========================================================================== */

#endif /* CRC_H */
