#include "../inc/framing_protocol.h"

/* ============================================================================================== */

#include <stdio.h>
#include <string.h>

/* ============================================================================================== */

#include "utilities.h"

/* ============================================================================================== */

#define RX_TIMEOUT_MS              5
#define TX_TIMEOUT_MS              30
#define FRAME_RETRANSMISSION_LIMIT 10

/* ============================================================================================== */

/* ============================================================================================== */

/* RX state machine implementation */

/* ============================================================================================== */

typedef frame_parser_state_t (*frame_parser_state_handler_t)(uint8_t const       byte,
                                                             framing_protocol_t* self);

typedef struct
{
    frame_parser_state_t         state;
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

/* ============================================================================================== */

/* Frame parser fsm states implementation */

/* ============================================================================================== */

static frame_parser_state_t rx_stx_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    if (!is_stx_valid(byte))
    {
        return STATE_DISCARD_FRAME;
    }
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_payload_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    ASSERT(self != NULL);
    self->rx_payload[self->rx_payload_index] = byte;
    self->rx_payload_index += 1;
    if (self->rx_payload_index == self->payload_size)
    {
        /* If payload is full, expect ETX */
        return STATE_PROCESS_ETX;
    }
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_etx_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    ASSERT(self != NULL);
    if (!is_etx_valid(byte))
    {
        return STATE_DISCARD_FRAME;
    }
    return STATE_PROCESS_BCC;
}

static frame_parser_state_t rx_bcc_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    self->_frame_bcc_buffer[self->_frame_bcc_index] = byte;
    self->_frame_bcc_index += 1;
    if (self->_frame_bcc_index >= BCC_SIZE_BYTES)
    {
        /* If BCC buffer is full, process and check BCC */
        uint16_t received_bcc
            = (uint16_t)(self->_frame_bcc_buffer[0] << 8) | (uint16_t)(self->_frame_bcc_buffer[1]);
        if (received_bcc
            == calculate_bcc(self->rx_buffer_bkp, FULL_FRAME_SIZE_BYTES - BCC_SIZE_BYTES))
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
    ASSERT(self != NULL);
    return STATE_DISCARD_FRAME;
}

static frame_parser_state_t rx_frame_ok_state_handler(uint8_t const byte, framing_protocol_t* self)
{
    ASSERT(self != NULL);
    return STATE_FRAME_OK;
}

/* ============================================================================================== */

/* Rx utilities */

/* ============================================================================================== */

static bool is_frame_valid(framing_protocol_t* self)
{
    ASSERT(self != NULL);

    /* Run state machine */
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        uint8_t byte = self->rx_buffer_bkp[i];
        self->state  = state_table[self->state].state_handler_function(byte, self);
    }

    /* Check last state */
    if (self->state == STATE_FRAME_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* ============================================================================================== */

/* Once the frame is processed and validated, this function transfers it to the public rx struct */
static void parse_payload_to_rx_data(framing_protocol_t* self, serial_protocol_t* serial_protocol)
{
    ASSERT(self != NULL);
    ASSERT(serial_protocol != NULL);

    for (uint8_t i = 0; i < FRAME_PAYLOAD_SIZE_BYTES; i++)
    {
        serial_protocol->rx_payload[i] = self->frame_payload_buffer[i];
    }
    serial_protocol->lost_frames_counter = lost_frames_counter;
}

/* ============================================================================================== */

/* Tx utilities */

/* ============================================================================================== */

static void reset_frame_transmitter(void)
{
    waiting_for_ack        = false;
    retransmission_counter = 0;
    for (uint8_t i = 0; i < TX_BUFFER_SIZE_BYTES; i++)
    {
        tx_buffer[i] = 0;
    }
}

/* ============================================================================================== */

static void handle_frame_transmission(void)
{
    retransmission_counter += 1;
    if (retransmission_counter > FRAME_RETRANSMISSION_LIMIT)
    {
        reset_frame_transmitter();
        return;
    }
    waiting_for_ack = true;
    UART_TRANSMIT_FRAME_DRIVER(tx_buffer, FULL_FRAME_SIZE_BYTES);
    SET_TIMEOUT_ON_DRIVER(TX_TIMEOUT_MS);
}

/* ============================================================================================== */

/* Callbacks */

/* ============================================================================================== */

/* This function is expected to be called inside the timeout timer ISR */
static void tmr_timeout_callback(void* context)
{
    framing_protocol_t* self = (framing_protocol_t*)context;
    if (self->_waiting_for_ack)
    {
        handle_frame_transmission();
        return;
    }
    /* If waiting for frame and timeout triggered, assume full frame reception */
    DISABLE_UART_RX_INTERRUPT_DRIVER();
    self->_frame_ready_for_val = true;
    return;
}

/* ============================================================================================== */

/* This function is expected to be called inside the serial protocol RX ISR */
static void uart_rx_byte_callback(const uint8_t* buffer, uint8_t length)
{
    SET_TIMEOUT_OFF_DRIVER();
    if (waiting_for_ack)
    {
        if (byte == ACK)
        {
            /* ACK received, reset retransmission counter */
            reset_frame_transmitter();
            return;
        }
        else
        {
            /* Retransmit */
            handle_frame_transmission();
            return;
        }
    }
    else
    {
        /* Fill rx_buffer with received byte */
        rx_buffer[rx_buffer_index] = byte;
        rx_buffer_index += 1;
        if (rx_buffer_index >= RX_BUFFER_SIZE_BYTES)
        {
            rx_buffer_index = 0;
        }
        SET_TIMEOUT_ON_DRIVER(RX_TIMEOUT_MS);
    }
    return;
}

/* ============================================================================================== */

/* Public functions */

/* ============================================================================================== */

/* This function is expected to be called continuosly to check for new data */
static bool serial_prot_retrieve_frame(framing_protocol_t* self)
{
    if (frame_ready_for_val)
    {
        frame_ready_for_val     = false;
        framing_protocol_t self = {
            .state                = STATE_PROCESS_STX,
            .frame_payload_index  = 0,
            ._frame_bcc_index     = 0,
            ._frame_bcc_buffer    = {0},
            .frame_payload_buffer = {0},
            .rx_buffer_bkp        = {0},
        };

        /* Make a copy of the uart rx buffer */
        for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
        {
            self.rx_buffer_bkp[i] = rx_buffer[i];
        }
        rx_buffer_index = 0; /* Reset */

        if (is_frame_valid(&self))
        {
            /* Parse payload to public interface. Re-enable uart for next reception */
            UART_TRANSMIT_BYTE_DRIVER(ACK);
            parse_payload_to_rx_data(&self, serial);
            ENABLE_UART_RX_INTERRUPT_DRIVER();
            return true;
        }
        else
        {
            UART_TRANSMIT_BYTE_DRIVER(NACK);
            lost_frames_counter += 1;
            ENABLE_UART_RX_INTERRUPT_DRIVER();
            return false;
        }
    }
    return false;
}

/* ============================================================================================== */

static void framing_protocol_transmit(framing_protocol_t* self)
{
    build_frame(serial->tx_payload, FRAME_PAYLOAD_SIZE_BYTES, tx_buffer, FULL_FRAME_SIZE_BYTES);
    handle_frame_transmission();
}

/* ============================================================================================== */

static void framing_protocol_initialize(framing_protocol_t* self)
{
    self->_uart->set_rx_callback(uart_rx_byte_callback);
    self->_timer->set_callback(tmr_timeout_callback);
}

/* ============================================================================================== */

int8_t framing_protocol_create(framing_protocol_t* self)
{
    return 0;
}

/* ============================================================================================== */
