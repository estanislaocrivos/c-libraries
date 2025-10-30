#ifndef CRC_H
#define CRC_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

struct crc
{
    /**
     * @brief CRC-8 polynomial.
     */
    uint8_t crc8_polynomial;

    /**
     * @brief CRC-8 initial value.
     */
    uint8_t crc8_initial_value;

    /**
     * @brief CRC-8 final XOR value.
     */
    uint8_t crc8_final_xor_value;

    /**
     * @brief Reflect input bytes.
     */
    bool reflect_input;

    /**
     * @brief Reflect output bytes.
     */
    bool reflect_output;
};

/* ============================================================================================== */

int8_t crc8_calculate(const struct crc* crc, const uint8_t* data, size_t length, uint8_t* result);

/* ============================================================================================== */

#endif /* CRC_H */
