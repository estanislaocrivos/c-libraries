#include "../inc/crc.h"

/* ============================================================================================== */

/* PRIVATE */

/* ============================================================================================== */

static inline uint8_t _reverse_8bits(uint8_t byte)
{
    byte = ((byte & 0xF0) >> 4) | ((byte & 0x0F) << 4);  // Swap nibbles
    byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);  // Swap pairs
    byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1);  // Swap bits
    return byte;
}

static inline uint16_t _reverse_16bits(uint16_t value)
{
    value = ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);  // Swap bytes
    value = ((value & 0xF0F0) >> 4) | ((value & 0x0F0F) << 4);  // Swap nibbles
    value = ((value & 0xCCCC) >> 2) | ((value & 0x3333) << 2);  // Swap pairs
    value = ((value & 0xAAAA) >> 1) | ((value & 0x5555) << 1);  // Swap bits
    return value;
}

/* ============================================================================================== */

/* PUBLIC */

/* ============================================================================================== */

int8_t crc8_calculate(const struct crc* crc, const uint8_t* data, size_t length, uint8_t* result)
{
    if (crc == NULL || data == NULL || result == NULL)
    {
        return -EFAULT;
    }
    if (length == 0)
    {
        return -EINVAL;
    }
    uint8_t crc_value = crc->crc8_initial_value;
    for (size_t i = 0; i < length; i++)
    {
        uint8_t byte = data[i];
        if (crc->reflect_input)
        {
            byte = _reverse_8bits(byte);
        }
        crc_value ^= byte;
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc_value & 0x80)
            {
                crc_value = (crc_value << 1) ^ crc->crc8_polynomial;
            }
            else
            {
                crc_value <<= 1;
            }
        }
    }
    if (crc->reflect_output)
    {
        crc_value = _reverse_8bits(crc_value);
    }
    crc_value ^= crc->crc8_final_xor_value;
    *result = crc_value;
    return 0;
}

/* ============================================================================================== */
