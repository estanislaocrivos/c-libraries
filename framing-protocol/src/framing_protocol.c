#include "../inc/framing_protocol.h"

/* ============================================================================================== */

#include <stdio.h>
#include <string.h>

/* ============================================================================================== */

#include "utilities.h"

#define STX_VALID_DEFAULT 0x02
#define ETX_VALID_DEFAULT 0x03
#define ACK_DEFAULT       0x06
#define NACK_DEFAULT      0x15

/* ============================================================================================== */

/* RX state machine implementation */

/* ============================================================================================== */

typedef frame_parser_state_t (*frame_parser_state_handler_t)(uint8_t const       byte,
                                                             framing_protocol_t* self);

typedef struct
{
    frame_parser_state_t         _state;
    frame_parser_state_handler_t state_handler_function;
} frame_parser_state_table_entry_t;

static frame_parser_state_t rx_stx_state_handler(uint8_t const byte, framing_protocol_t* self);
static frame_parser_state_t rx_payload_state_handler(uint8_t const byte, framing_protocol_t* self);
static frame_parser_state_t rx_etx_state_handler(uint8_t const byte, framing_protocol_t* self);
static frame_parser_state_t rx_bcc_state_handler(uint8_t const byte, framing_protocol_t* self);
static frame_parser_state_t rx_discard_frame_state_handler(uint8_t const       byte,
                                                           framing_protocol_t* self);
static frame_parser_state_t rx_frame_ok_state_handler(uint8_t const byte, framing_protocol_t* self);

/* State table. May be modified to change dynamics */
static const frame_parser_state_table_entry_t state_table[] = {
    {STATE_PROCESS_STX, rx_stx_state_handler},
    {STATE_PROCESS_DATA, rx_payload_state_handler},
    {STATE_PROCESS_ETX, rx_etx_state_handler},
    {STATE_PROCESS_BCC, rx_bcc_state_handler},
    {STATE_DISCARD_FRAME, rx_discard_frame_state_handler},
    {STATE_FRAME_OK, rx_frame_ok_state_handler},
};

static frame_parser_state_t rx_stx_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    if (!is_stx_valid(self, byte))
    {
        return STATE_DISCARD_FRAME;
    }
    self->_rx_buffer[0]    = byte;
    self->_rx_buffer_index = 1;
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_payload_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    self->_rx_buffer[self->_rx_buffer_index] = byte;
    self->_rx_buffer_index += 1;
    if (self->_rx_buffer_index == self->payload_size + 1)
    {
        /* If payload is full, expect ETX */
        return STATE_PROCESS_ETX;
    }
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_etx_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    if (!is_etx_valid(self, byte))
    {
        return STATE_DISCARD_FRAME;
    }
    self->_rx_buffer[self->_rx_buffer_index] = byte;
    self->_rx_buffer_index += 1;
    return STATE_PROCESS_BCC;
}

static frame_parser_state_t rx_bcc_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    self->_bcc_buffer[self->_bcc_buffer_index] = byte;
    self->_bcc_buffer_index += 1;
    if (self->_bcc_buffer_index >= BCC_SIZE_BYTES)
    {
        uint16_t received_bcc
            = (uint16_t)(self->_bcc_buffer[0] << 8) | (uint16_t)(self->_bcc_buffer[1]);
        if (received_bcc
            == self->checksum_calculator(self->_rx_buffer, self->_rx_buffer_index - BCC_SIZE_BYTES))
        {
            return STATE_FRAME_OK;
        }
        else
        {
            return STATE_DISCARD_FRAME;
        }
    }
    return STATE_PROCESS_BCC;
}

static frame_parser_state_t rx_discard_frame_state_handler(uint8_t const       byte,
                                                           framing_protocol_t* self)
{
    return STATE_DISCARD_FRAME;
}

static frame_parser_state_t rx_frame_ok_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    return STATE_FRAME_OK;
}

/* ============================================================================================== */

/* Rx utilities */

/* ============================================================================================== */

static bool is_frame_valid(framing_protocol_t* self)
{
    if (self->_state == STATE_FRAME_OK)
    {
        return true;
    }
    return false;
}

/* ============================================================================================== */

/* Tx utilities */

/* ============================================================================================== */

static void reset_framing_protocol(framing_protocol_t* self)
{
    self->_state                = STATE_PROCESS_STX;
    self->_rx_buffer_index      = 0;
    self->_tx_buffer_index      = 0;
    self->_bcc_buffer_index     = 0;
    self->_waiting_for_ack      = false;
    self->_receiving_frame      = false;
    self->lost_frames_counter   = 0;
    self->transmissions_counter = 0;
}

/* ============================================================================================== */

static void handle_frame_transmission(framing_protocol_t* self)
{
    if (self->_initialized)
    {
        self->transmissions_counter += 1;
        if (self->transmissions_counter > self->max_retransmissions)
        {
            reset_framing_protocol(self);
            return;
        }
        self->_waiting_for_ack = true;
        self->_uart->ops->transmit(self->_uart, self->_tx_buffer, self->_tx_buffer_index);
        self->_timer->ops->set_timeout_ms(self->_timer, self->tx_timeout_ms);
    }
}

static void build_frame(framing_protocol_t* self, const uint8_t* payload, size_t size)
{
    if (self->_initialized)
    {
        if (size > MAX_PAYLOAD_SIZE)
        {
            return;
        }
        uint8_t index             = 0;
        self->_tx_buffer[index++] = self->_stx_byte;
        for (uint8_t i = 0; i < size; i++)
        {
            self->_tx_buffer[index++] = payload[i];
        }
        self->_tx_buffer[index++] = self->_etx_byte;
        uint16_t bcc              = self->checksum_calculator(self->_tx_buffer, index);
        self->_tx_buffer[index++] = (bcc >> 8) & 0xFF;
        self->_tx_buffer[index++] = bcc & 0xFF;
        self->_tx_buffer_index    = index;
    }
}

/* ============================================================================================== */

/* Callbacks */

/* ============================================================================================== */

/* This function is expected to be called inside the timeout timer ISR */
static void tmr_timeout_callback(void* context)
{
    framing_protocol_t* self = (framing_protocol_t*)context;
    if (self->_initialized)
    {
        if (self->_waiting_for_ack)
        {
            handle_frame_transmission(self);
            return;
        }
        /* If waiting for frame and timeout triggered, assume full frame reception */
        self->_uart->ops->enable_rx_interrupt(self->_uart, false);
    }
    return;
}

/* ============================================================================================== */

/* This function is expected to be called inside the serial protocol RX ISR */
static void uart_rx_byte_callback(const uint8_t* buffer, uint8_t size, void* context)
{
    framing_protocol_t* self = (framing_protocol_t*)context;
    if (self->_initialized)
    {
        self->_timer->ops->deactivate_timeout(self->_timer);
        if (self->_waiting_for_ack)
        {
            if (buffer == self->_ack_byte)
            {
                /* ACK received, reset retransmission counter */
                reset_framing_protocol(self);
                return;
            }
            else
            {
                /* Retransmit */
                handle_frame_transmission(self);
                return;
            }
        }
        else
        {
            /* Fill rx_buffer with received byte */
            self->_state = state_table[self->_state].state_handler_function(buffer, self);
            if (self->_rx_buffer_index == MAX_FRAME_SIZE)
            {
                self->_rx_buffer_index = 0;
            }
            self->_timer->ops->set_timeout_ms(self->_timer, self->rx_timeout_ms);
        }
    }
    return;
}

/* ============================================================================================== */

/* Public functions */

/* ============================================================================================== */

/* This function is expected to be called continuosly to check for new data */
int8_t framing_protocol_receive_payload(framing_protocol_t* self, uint8_t* payload, uint8_t size)
{
    if (self->_initialized)
    {
        if (is_frame_valid(&self))
        {
            /* Parse payload to public interface. Re-enable uart for next reception */
            self->_uart->ops->transmit(self->_uart, self->_ack_byte, 1);
            for (uint8_t i = 0; i < self->payload_size; i++)
            {
                payload[i] = self->_rx_buffer[i + 1];
            }
            self->_uart->ops->enable_rx_interrupt(self->_uart, true);
            return 0;
        }
        else
        {
            self->_uart->ops->transmit(self->_uart, self->_nack_byte, 1);
            self->lost_frames_counter += 1;
            reset_framing_protocol(self);
            self->_uart->ops->enable_rx_interrupt(self->_uart, true);
        }
    }
    return -EBUSY;
}

/* ============================================================================================== */

int8_t framing_protocol_transmit_payload(framing_protocol_t* self,
                                         const uint8_t*      payload,
                                         uint8_t             size)
{
    if (self->_initialized)
    {
        build_frame(self, payload, size);
        handle_frame_transmission(self);
    }
    return 0;
}

/* ============================================================================================== */

int8_t framing_protocol_initialize(framing_protocol_t* self)
{
    self->_uart->ops->set_rx_callback(self->_uart, uart_rx_byte_callback, self);
    self->_timer->ops->set_timeout_callback(self->_timer, tmr_timeout_callback, self);
    self->_initialized = true;
    return 0;
}

/* ============================================================================================== */

int8_t framing_protocol_create(framing_protocol_t* self)
{
    self->_initialized = false;
    if (!self->_stx_byte)
    {
        self->_stx_byte = STX_VALID_DEFAULT;
    }
    if (!self->_etx_byte)
    {
        self->_etx_byte = ETX_VALID_DEFAULT;
    }
    if (!self->_ack_byte)
    {
        self->_ack_byte = ACK_DEFAULT;
    }
    if (!self->_nack_byte)
    {
        self->_nack_byte = NACK_DEFAULT;
    }

    return 0;
}

/* ============================================================================================== */
