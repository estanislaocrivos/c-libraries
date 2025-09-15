#include "../inc/framing_protocol.h"

/* ============================================================================================== */

#include <stdio.h>
#include <string.h>

/* ============================================================================================== */

#include "utilities.h"

/* ============================================================================================== */

#define RX_TIMEOUT_MS                    5
#define TX_TIMEOUT_MS                    30
#define FRAME_RETRANSMISSION_LIMIT       10

/* ============================================================================================== */

#define UART_INITIALIZE_DRIVER           uart1_initialize
#define UART_TRANSMIT_BYTE_DRIVER        uart1_transmit_byte
#define UART_TRANSMIT_FRAME_DRIVER       uart1_transmit_buffer
#define UART_SET_CALLBACK_DRIVER         uart1_set_rx_callback
#define DISABLE_UART_RX_INTERRUPT_DRIVER uart1_disable_rx_interrupt
#define ENABLE_UART_RX_INTERRUPT_DRIVER  uart1_enable_rx_interrupt

#define TMR_INITIALIZE_DRIVER            tmr0_initialize
#define TIMEOUT_TIMER_SET_CALLBACK       tmr0_set_timeout_callback
#define SET_TIMEOUT_ON_DRIVER            tmr0_set_timeout_ms
#define SET_TIMEOUT_OFF_DRIVER           tmr0_deactivate_timeout

#define RX_BUFFER_SIZE_BYTES             32
#define TX_BUFFER_SIZE_BYTES             FULL_FRAME_SIZE_BYTES

static volatile bool    waiting_for_ack                 = false;
static volatile bool    frame_ready_for_val             = false;
static volatile uint8_t lost_frames_counter             = 0;
static volatile uint8_t retransmission_counter          = 0;
static volatile uint8_t rx_buffer_index                 = 0;
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE_BYTES] = {0};
static uint8_t          tx_buffer[TX_BUFFER_SIZE_BYTES] = {0};

/* ============================================================================================== */

typedef struct
{
    /* Buffer & indexes */
    uint8_t rx_buffer_bkp[FULL_FRAME_SIZE_BYTES]; /* Backup buffer for rx */

    /* Parser fsm current state */
    frame_parser_state_t state;

    /* Buffer for frame procesing */
    uint8_t frame_payload_buffer[FRAME_PAYLOAD_SIZE_BYTES];
    uint8_t frame_payload_index;
    uint8_t frame_bcc_buffer[BCC_SIZE_BYTES];
    uint8_t frame_bcc_index;
} frame_parser_ctxt_t;

/* ============================================================================================== */

/* RX state machine implementation */

/* ============================================================================================== */

typedef frame_parser_state_t (*frame_parser_state_handler_t)(uint8_t const        byte,
                                                             frame_parser_ctxt_t* frame_parser);

typedef struct
{
    frame_parser_state_t         state;
    frame_parser_state_handler_t state_handler_function;
} frame_parser_state_table_entry_t;

static frame_parser_state_t rx_stx_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser);
static frame_parser_state_t rx_data_state_handler(uint8_t const        byte,
                                                  frame_parser_ctxt_t* frame_parser);
static frame_parser_state_t rx_etx_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser);
static frame_parser_state_t rx_bcc_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser);
static frame_parser_state_t rx_discard_frame_state_handler(uint8_t const        byte,
                                                           frame_parser_ctxt_t* frame_parser);
static frame_parser_state_t rx_frame_ok_state_handler(uint8_t const        byte,
                                                      frame_parser_ctxt_t* frame_parser);

/* State table. May be modified to change dynamics */
static const frame_parser_state_table_entry_t state_table[] = {
    {STATE_PROCESS_STX, rx_stx_state_handler},
    {STATE_PROCESS_DATA, rx_data_state_handler},
    {STATE_PROCESS_ETX, rx_etx_state_handler},
    {STATE_PROCESS_BCC, rx_bcc_state_handler},
    {STATE_DISCARD_FRAME, rx_discard_frame_state_handler},
    {STATE_FRAME_OK, rx_frame_ok_state_handler},
};

/* ============================================================================================== */

/* Frame parser fsm states implementation */

/* ============================================================================================== */

static frame_parser_state_t rx_stx_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    if (!is_stx_valid(byte))
    {
        return STATE_DISCARD_FRAME;
    }
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_data_state_handler(uint8_t const        byte,
                                                  frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    frame_parser->frame_payload_buffer[frame_parser->frame_payload_index] = byte;
    frame_parser->frame_payload_index += 1;
    if (frame_parser->frame_payload_index >= FRAME_PAYLOAD_SIZE_BYTES)
    {
        /* If payload is full, expect ETX */
        return STATE_PROCESS_ETX;
    }
    return STATE_PROCESS_DATA;
}

static frame_parser_state_t rx_etx_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    if (!is_etx_valid(byte))
    {
        return STATE_DISCARD_FRAME;
    }
    return STATE_PROCESS_BCC;
}

static frame_parser_state_t rx_bcc_state_handler(uint8_t const        byte,
                                                 frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    frame_parser->frame_bcc_buffer[frame_parser->frame_bcc_index] = byte;
    frame_parser->frame_bcc_index += 1;
    if (frame_parser->frame_bcc_index >= BCC_SIZE_BYTES)
    {
        /* If BCC buffer is full, process and check BCC */
        uint16_t received_bcc = (uint16_t)(frame_parser->frame_bcc_buffer[0] << 8)
                                | (uint16_t)(frame_parser->frame_bcc_buffer[1]);
        if (received_bcc
            == calculate_bcc(frame_parser->rx_buffer_bkp, FULL_FRAME_SIZE_BYTES - BCC_SIZE_BYTES))
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

static frame_parser_state_t rx_discard_frame_state_handler(uint8_t const        byte,
                                                           frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    return STATE_DISCARD_FRAME;
}

static frame_parser_state_t rx_frame_ok_state_handler(uint8_t const        byte,
                                                      frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);
    return STATE_FRAME_OK;
}

/* ============================================================================================== */

/* Rx utilities */

/* ============================================================================================== */

static bool is_frame_valid(frame_parser_ctxt_t* frame_parser)
{
    ASSERT(frame_parser != NULL);

    /* Run state machine */
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        uint8_t byte = frame_parser->rx_buffer_bkp[i];
        frame_parser->state
            = state_table[frame_parser->state].state_handler_function(byte, frame_parser);
    }

    /* Check last state */
    if (frame_parser->state == STATE_FRAME_OK)
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
static void parse_payload_to_rx_data(frame_parser_ctxt_t* frame_parser,
                                     serial_protocol_t*   serial_protocol)
{
    ASSERT(frame_parser != NULL);
    ASSERT(serial_protocol != NULL);

    for (uint8_t i = 0; i < FRAME_PAYLOAD_SIZE_BYTES; i++)
    {
        serial_protocol->rx_payload[i] = frame_parser->frame_payload_buffer[i];
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
static void tmr_timeout_callback(void)
{
    if (waiting_for_ack)
    {
        handle_frame_transmission();
        return;
    }
    /* If waiting for frame and timeout triggered, assume full frame reception */
    DISABLE_UART_RX_INTERRUPT_DRIVER();
    frame_ready_for_val = true;
    return;
}

/* ============================================================================================== */

/* This function is expected to be called inside the serial protocol RX ISR */
static void uart_rx_byte_callback(uint8_t byte)
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
static bool serial_prot_retrieve_frame(void* self)
{
    ASSERT(self != NULL);
    serial_protocol_t* serial = (serial_protocol_t*)self;

    if (frame_ready_for_val)
    {
        frame_ready_for_val              = false;
        frame_parser_ctxt_t frame_parser = {
            .state                = STATE_PROCESS_STX,
            .frame_payload_index  = 0,
            .frame_bcc_index      = 0,
            .frame_bcc_buffer     = {0},
            .frame_payload_buffer = {0},
            .rx_buffer_bkp        = {0},
        };

        /* Make a copy of the uart rx buffer */
        for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
        {
            frame_parser.rx_buffer_bkp[i] = rx_buffer[i];
        }
        rx_buffer_index = 0; /* Reset */

        if (is_frame_valid(&frame_parser))
        {
            /* Parse payload to public interface. Re-enable uart for next reception */
            UART_TRANSMIT_BYTE_DRIVER(ACK);
            parse_payload_to_rx_data(&frame_parser, serial);
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

static void serial_prot_transmit_frame(void* self)
{
    ASSERT(self != NULL);
    serial_protocol_t* serial = (serial_protocol_t*)self;

    build_frame(serial->tx_payload, FRAME_PAYLOAD_SIZE_BYTES, tx_buffer, FULL_FRAME_SIZE_BYTES);
    handle_frame_transmission();
}

/* ============================================================================================== */

static void serial_prot_initialize(void* self)
{
    ASSERT(self != NULL);
    serial_protocol_t* serial = (serial_protocol_t*)self;

    /* Assumes that UART1 and TMR0 and initialized before this call */
    UART_SET_CALLBACK_DRIVER(uart_rx_byte_callback);
    TIMEOUT_TIMER_SET_CALLBACK(tmr_timeout_callback);
    reset_frame_transmitter();
}

/* ============================================================================================== */

serial_protocol_t serial_protocol_create(void)
{
    serial_protocol_t serial_protocol = {
        .lost_frames_counter = 0,
        .rx_payload          = {0},
        .tx_payload          = {0},
        .initialize          = serial_prot_initialize,
        .transmit_payload    = serial_prot_transmit_frame,
        .receive_payload     = serial_prot_retrieve_frame,
    };
    return serial_protocol;
}

/* ============================================================================================== */
