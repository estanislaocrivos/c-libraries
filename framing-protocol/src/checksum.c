#include "utilities.h"

/* ============================================================================================== */

uint16_t basic_checksum(const uint8_t* buffer, uint8_t length)
{
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
