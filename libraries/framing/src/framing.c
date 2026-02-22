#include "../inc/framing.h"

/* ========================================================================== */

#include <string.h>

/* ========================================================================== */

/* PRIVATE */

/* ========================================================================== */

// RX state machine implementation. Each state takes the instance and the
// incoming byte and processes it, going one state forward or resetting the FSM

typedef enum framing_state (*framing_state_handler_t)(
    struct framing* self, uint8_t byte);

typedef struct
{
    framing_state_handler_t handler;
} framing_state_table_entry_t;

static enum framing_state start_state_handler(
    struct framing* self, uint8_t byte);
static enum framing_state length_state_handler(
    struct framing* self, uint8_t byte);
static enum framing_state payload_state_handler(
    struct framing* self, uint8_t byte);
static enum framing_state stop_state_handler(struct framing* self, uint8_t byte);
static enum framing_state crc_state_handler(struct framing* self, uint8_t byte);

// State table mapping states to their handlers
static const framing_state_table_entry_t state_table[] = {
    {start_state_handler},
    {length_state_handler},
    {payload_state_handler},
    {stop_state_handler},
    {crc_state_handler},
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

static enum framing_state length_state_handler(
    struct framing* self, uint8_t byte)
{
    if (byte == 0 || byte > self->max_payload_size)
    {
        return FRAMING_START_STATE;  // Invalid length, reset FSM
    }
    self->payload_size = byte;
    buffer_push(self->parsing_buffer, byte);
    return FRAMING_PAYLOAD_STATE;
}

static enum framing_state payload_state_handler(
    struct framing* self, uint8_t byte)
{
    buffer_push(self->parsing_buffer, byte);
    size_t index;
    buffer_get_index(self->parsing_buffer, &index);
    if (index == self->payload_size + 2)  // +2 for start delimiter and length
                                          // byte
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
    uint8_t  calculated_crc;
    uint8_t* raw;
    size_t   index;
    buffer_get_raw(self->parsing_buffer, &raw);
    buffer_get_index(self->parsing_buffer, &index);
    crc8_calculate(self->crc8_calculator, raw, index, &calculated_crc);
    if (byte == calculated_crc)
    {
        return FRAMING_COMPLETE_STATE;
    }
    return FRAMING_ERROR_STATE;
}

/* ========================================================================== */

/* PUBLIC */

/* ========================================================================== */

int8_t framing_init(struct framing* self)
{
    if (self == NULL || self->crc8_calculator == NULL
        || self->rx_raw_buffer == NULL || self->parsing_buffer == NULL
        || self->tx_frame_buffer == NULL)
    {
        return -EFAULT;
    }
    self->current_state   = FRAMING_START_STATE;
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t framing_process_incoming_data(struct framing* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
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
    self->current_state = state_table[self->current_state].handler(self, byte);
    if (self->current_state == FRAMING_COMPLETE_STATE)
    {
        self->frame_available = true;
        return 0;
    }
    else if (self->current_state == FRAMING_ERROR_STATE)
    {
        self->lost_frames += 1;
        self->current_state = FRAMING_START_STATE;  // Resets FSM upon erroneous
                                                    // CRC
        return -EILSEQ;
    }
    return -EAGAIN;
}

/* ========================================================================== */

int8_t framing_retrieve_payload(
    struct framing* self, uint8_t* payload, uint8_t* payload_size)
{
    if (self == NULL || payload == NULL || payload_size == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (self->frame_available)
    {
        *payload_size = self->payload_size;
        for (size_t i = 0; i < self->payload_size; i++)
        {
            uint8_t byte;
            buffer_at(
                self->parsing_buffer,
                i + 2,
                &byte);  // +2 to skip start delimiter and length
            payload[i] = byte;
        }
        self->frame_available = false;
        self->current_state   = FRAMING_START_STATE;  // Resets FSM upon
                                                      // successful retrieval
        return 0;
    }
    return -ENODATA;
}

/* ========================================================================== */

int8_t framing_build_frame(
    struct framing* self,
    const uint8_t*  payload,
    uint8_t         payload_size,
    uint8_t*        frame_size)
{
    if (self == NULL || payload == NULL || frame_size == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
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
    uint8_t  crc;
    uint8_t* raw;
    size_t   index;
    buffer_get_raw(self->tx_frame_buffer, &raw);
    buffer_get_index(self->tx_frame_buffer, &index);
    crc8_calculate(self->crc8_calculator, raw, index, &crc);
    if (buffer_push(self->tx_frame_buffer, crc))
    {
        return -ENOBUFS;
    }
    buffer_get_index(self->tx_frame_buffer, &index);
    *frame_size = (uint8_t)index;
    return 0;
}

/* ========================================================================== */
