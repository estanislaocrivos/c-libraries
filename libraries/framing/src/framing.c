#include "../inc/framing.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

/* PRIVATE */

/* ============================================================================================== */

/* RX state machine implementation */

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

/* State table. State handlers are in order according to the enum framing_state enum */
static const framing_state_table_entry_t state_table[] = {
    {start_state_handler}, {length_state_handler}, {payload_state_handler},
    {stop_state_handler},  {crc_state_handler},
};

static enum framing_state start_state_handler(struct framing* self, uint8_t byte)
{
    if (byte != self->start_delimiter)
    {
        return FRAMING_START_STATE;
    }
    buffer_reset_index(self->parsing_buffer);
    buffer_push(self->parsing_buffer, byte);
    return FRAMING_LENGTH_STATE;
}

static enum framing_state length_state_handler(struct framing* self, uint8_t byte)
{
    self->_payload_size = byte;
    buffer_push(self->parsing_buffer, byte);
    return FRAMING_PAYLOAD_STATE;
}

static enum framing_state payload_state_handler(struct framing* self, uint8_t byte)
{
    buffer_push(self->parsing_buffer, byte);
    if (self->parsing_buffer->index
        == self->_payload_size + 2)  // +2 for start delimiter and length byte
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
        return FRAMING_START_STATE;  // Invalid frame, reset FSM
    }
    buffer_push(self->parsing_buffer, byte);
    return FRAMING_CRC_STATE;
}

static enum framing_state crc_state_handler(struct framing* self, uint8_t byte)
{
    uint8_t calculated_crc;
    crc8_calculate(self->crc8_calculator, self->parsing_buffer->buffer, self->parsing_buffer->index,
                   &calculated_crc);
    if (byte == calculated_crc)
    {
        return FRAMING_COMPLETE_STATE;
    }
    return FRAMING_ERROR_STATE;
}

/* ============================================================================================== */

/* PUBLIC */

/* ============================================================================================== */

int8_t framing_init(struct framing* self)
{
    if (self == NULL || self->crc8_calculator == NULL)
    {
        return -EFAULT;
    }
    if (self->parsing_buffer == NULL || self->parsing_buffer->buffer == NULL)
    {
        return -EFAULT;
    }
    if (self->tx_frame_buffer == NULL || self->tx_frame_buffer->buffer == NULL)
    {
        return -EFAULT;
    }
    if (self->rx_raw_buffer == NULL || self->rx_raw_buffer->buffer == NULL)
    {
        return -EFAULT;
    }
    self->_current_state   = FRAMING_START_STATE;
    self->_was_initialized = true;
    return 0;
}

/* ============================================================================================== */

int8_t process_incoming_data(struct framing* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
    uint8_t byte;
    // Retrieve next byte from RX raw buffer
    if (ring_buffer_pop(self->rx_raw_buffer, &byte, 1))
    {
        return -ENODATA;
    }
    // Process byte through state machine
    self->_current_state = state_table[self->_current_state].handler(self, byte);
    if (self->_current_state == FRAMING_COMPLETE_STATE
        || self->_current_state == FRAMING_ERROR_STATE)
    {
        return 0;
    }
    return -EAGAIN;
}

/* ============================================================================================== */

int8_t retrieve_payload(struct framing* self, uint8_t* payload, uint8_t* payload_size)
{
    switch (self->_current_state)
    {
        case FRAMING_COMPLETE_STATE:
        {
            *payload_size = self->_payload_size;
            for (size_t i = 0; i < self->_payload_size; i++)
            {
                payload[i] = self->parsing_buffer
                                 ->buffer[i + 2];  // Offset by 2 to skip start delimiter and length
            }
            self->_current_state = FRAMING_START_STATE;  // Resets FSM upon successful retrieval
            return 0;
        }
        case FRAMING_ERROR_STATE:
        {
            self->_current_state = FRAMING_START_STATE;  // Resets FSM upon erroneous CRC
            return -EILSEQ;
        }
        default:
        {
            return -ENODATA;
        }
    }
}

/* ============================================================================================== */

int8_t build_frame(struct framing* self,
                   const uint8_t*  payload,
                   uint8_t         payload_size,
                   uint8_t*        frame_size)
{
    if (self == NULL || payload == NULL || frame_size == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
    if ((payload_size + 4) > self->tx_frame_buffer->size)  // 4 bytes for delimiters, length and crc
    {
        return -ENOBUFS;
    }
    buffer_reset_index(self->tx_frame_buffer);
    if (buffer_push(self->tx_frame_buffer, self->start_delimiter)
        || buffer_push(self->tx_frame_buffer, payload_size))
    {
        return -ENOBUFS;
    }
    for (size_t i = 0; i < payload_size; i++)
    {
        if (buffer_push(self->tx_frame_buffer, payload[i]))
        {
            return -ENOBUFS;
        }
    }
    if (buffer_push(self->tx_frame_buffer, self->stop_delimiter))
    {
        return -ENOBUFS;
    }
    uint8_t crc;
    crc8_calculate(self->crc8_calculator, self->tx_frame_buffer->buffer,
                   self->tx_frame_buffer->index, &crc);
    if (buffer_push(self->tx_frame_buffer, crc))
    {
        return -ENOBUFS;
    }
    *frame_size = self->tx_frame_buffer->index;
    return 0;
}

/* ============================================================================================== */
