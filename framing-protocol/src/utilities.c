#include "../inc/utilities.h"

/* ============================================================================================== */

bool is_stx_valid(const framing_protocol_t* self, uint8_t byte)
{
    return (byte == self->_stx_byte);
}

/* ============================================================================================== */

bool is_etx_valid(const framing_protocol_t* self, uint8_t byte)
{
    return (byte == self->_etx_byte);
}

/* ============================================================================================== */
