#ifndef FRAMING_PROTOCOL_H
#define FRAMING_PROTOCOL_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ============================================================================================== */

#include "errno.h"
#include "../../uart-interface/inc/uart.h"
#include "../../mcu-mngr-interface/inc/mcu_manager.h"

/* ============================================================================================== */

#define MAX_PAYLOAD_SIZE  16
#define MAX_OVERHEAD_SIZE 5

typedef enum
{
    STATE_PROCESS_STX,
    STATE_PROCESS_DATA,
    STATE_PROCESS_ETX,
    STATE_PROCESS_BCC,
    STATE_DISCARD_FRAME,
    STATE_FRAME_OK
} frame_parser_state_t;

typedef struct
{
    uart_t*              _uart;
    timer_interface_t*   _timer;
    frame_parser_state_t _state;
    uint8_t              _rx_buffer[MAX_PAYLOAD_SIZE + MAX_OVERHEAD_SIZE];
    uint8_t              _tx_buffer[MAX_PAYLOAD_SIZE + MAX_OVERHEAD_SIZE];
    uint8_t              _rx_buffer_index;
    uint8_t              _tx_buffer_index;
    uint8_t              _retransmission_counter;
    uint8_t              _frame_bcc_buffer[2];
    uint8_t              _frame_bcc_index;
    bool                 _waiting_for_ack;

    uint8_t* rx_payload;
    uint8_t  rx_payload_index;

    uint8_t* tx_payload;
    uint8_t  payload_size;

    bool    receiving_frame;
    uint8_t lost_frames;
    uint8_t stx_byte;
    uint8_t etx_byte;

    uint16_t (*checksum_calculator)(const uint8_t*, uint8_t);

} framing_protocol_t;

/* ============================================================================================== */

int8_t framing_protocol_create(framing_protocol_t* self);

/* ============================================================================================== */

#endif /* FRAMING_PROTOCOL_H */
