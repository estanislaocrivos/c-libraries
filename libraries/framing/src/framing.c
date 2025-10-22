#include "../inc/framing.h"

/* ============================================================================================== */

/* RX state machine implementation */

/* ============================================================================================== */

static void _fill_internal_buffer(struct framing* self, uint8_t byte)
{
    if (self->_buffer_index < self->_buffer_size)
    {
        self->_buffer[self->_buffer_index] = byte;
        self->_buffer_index += 1;
    }
}

typedef enum framing_state (*framing_state_handler_t)(struct framing* self, uint8_t byte);

typedef struct
{
    framing_state_handler_t handler;
} framing_state_table_entry_t;

static enum framing_state start_state_handler(struct framing* self, uint8_t byte);
static enum framing_state length_state_handler(struct framing* self, uint8_t byte);
static enum framing_state payload_state_handler(struct framing* self, uint8_t byte);
static enum framing_state stop_state_handler(struct framing* self, uint8_t byte);
static enum framing_state crc_state_handler(struct framing* self, uint8_t byte);
static enum framing_state complete_state_handler(struct framing* self, uint8_t byte);
static enum framing_state error_state_handler(struct framing* self, uint8_t byte);

/* State table. State handlers are in order according to the enum framing_state enum */
static const framing_state_table_entry_t state_table[] = {
    {start_state_handler}, {length_state_handler},   {payload_state_handler}, {stop_state_handler},
    {crc_state_handler},   {complete_state_handler}, {error_state_handler},
};

static enum framing_state start_state_handler(struct framing* self, uint8_t byte)
{
    if (byte != self->start_delimiter)
    {
        return FRAMING_ERROR_STATE;
    }
    _fill_internal_buffer(self, byte);
    return FRAMING_LENGTH_STATE;
}

static enum framing_state length_state_handler(struct framing* self, uint8_t byte)
{
    self->_payload_size = (size_t)byte;
    _fill_internal_buffer(self, byte);
    return FRAMING_PAYLOAD_STATE;
}

static enum framing_state payload_state_handler(struct framing* self, uint8_t byte)
{
    _fill_internal_buffer(self, byte);
    if (self->_buffer_index == self->_payload_size + 1)
    {
        /* If payload is full, expect ETX */
        return FRAMING_STOP_STATE;
    }
    return FRAMING_PAYLOAD_STATE;
}

static enum framing_state stop_state_handler(struct framing* self, uint8_t byte)
{
    if (byte != self->stop_delimiter)
    {
        return FRAMING_ERROR_STATE;
    }
    _fill_internal_buffer(self, byte);
    return FRAMING_CRC_STATE;
}

static enum framing_state crc_state_handler(struct framing* self, uint8_t byte)
{
    uint8_t calculated_crc;
    crc8_calculate(self->crc_calculator, self->_buffer, self->_buffer_index, &calculated_crc);
    if (byte == calculated_crc)
    {
        return FRAMING_COMPLETE_STATE;
    }
    return FRAMING_ERROR_STATE;
}

static enum framing_state error_state_handler(struct framing* self, uint8_t byte)
{
    return FRAMING_ERROR_STATE;
}

static enum framing_state complete_state_handler(struct framing* self, uint8_t byte)
{
    return FRAMING_COMPLETE_STATE;
}

/* ============================================================================================== */

int8_t retrieve_payload(struct framing* self, uint8_t* payload, size_t payload_size)
{
    if (self == NULL || payload == NULL || payload_size == NULL)
    {
        return -EFAULT;
    }
    if (pop(self->rx_ring_buffer, payload, payload_size))
    {
        return -EIO;
    }
}

/* ============================================================================================== */
