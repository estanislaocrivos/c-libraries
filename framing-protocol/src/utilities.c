#include "utilities.h"

/* ============================================================================================== */

#include "../../../inc/debug.h"

/* ============================================================================================== */

uint16_t calculate_bcc(const uint8_t* buffer, uint8_t length)
{
    ASSERT(buffer != NULL);
    if (buffer == NULL || length == 0)
    {
        return 0;
    }
    uint16_t bcc = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        bcc += buffer[i];
    }
    return bcc;
}

/* ============================================================================================== */

uint8_t build_frame(const uint8_t* payload,
                    uint8_t        payload_size_bytes,
                    uint8_t*       out_buffer,
                    uint8_t        out_buffer_size_bytes)
{
    ASSERT(payload != NULL);
    ASSERT(out_buffer != NULL);
    if (payload == NULL || out_buffer == NULL || payload_size_bytes == 0)
    {
        return 0;
    }
    if (out_buffer_size_bytes < (payload_size_bytes + 4))  // 1 for STX, 1 for ETX, 2 for BCC
    {
        return 0;
    }
    uint8_t index       = 0;
    out_buffer[index++] = STX_VALID;
    for (uint8_t i = 0; i < payload_size_bytes; i++)
    {
        out_buffer[index++] = payload[i];
    }
    out_buffer[index++] = ETX_VALID;
    uint16_t bcc        = calculate_bcc(out_buffer, index);
    out_buffer[index++] = (bcc >> 8) & 0xFF;
    out_buffer[index++] = bcc & 0xFF;
    return index;
}

/* ============================================================================================== */

bool is_stx_valid(uint8_t byte)
{
    return (byte == STX_VALID);
}

/* ============================================================================================== */

bool is_etx_valid(uint8_t byte)
{
    return (byte == ETX_VALID);
}

/* ============================================================================================== */
