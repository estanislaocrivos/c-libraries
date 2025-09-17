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
#define MAX_OVERHEAD_SIZE 4 /* STX, ETX, 2x BCC */
#define MAX_FRAME_SIZE    (MAX_PAYLOAD_SIZE + MAX_OVERHEAD_SIZE)

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
    struct uart_port*    _uart;
    struct timer*        _timer;
    frame_parser_state_t _state;

    volatile uint8_t _rx_byte;

    uint8_t _rx_buffer[MAX_FRAME_SIZE];
    uint8_t _rx_buffer_index;

    uint8_t _tx_buffer[MAX_FRAME_SIZE];
    uint8_t _tx_buffer_index;

    uint8_t _retransmission_counter;
    uint8_t _frame_bcc_buffer[2];
    uint8_t _frame_bcc_index;
    bool    _waiting_for_ack;

    bool    _receiving_frame;
    uint8_t _lost_frames;
    uint8_t _stx_byte;
    uint8_t _etx_byte;

    uint16_t (*checksum_calculator)(const uint8_t*, uint8_t);

} framing_protocol_t;

/* ============================================================================================== */

int8_t framing_protocol_create(framing_protocol_t* self);

/* ============================================================================================== */

int8_t framing_protocol_initialize(framing_protocol_t* self);

/* ============================================================================================== */

int8_t framing_protocol_transmit_payload(framing_protocol_t* self,
                                         const uint8_t*      payload,
                                         uint8_t             size);

/* ============================================================================================== */

bool framing_protocol_receive_payload(framing_protocol_t* self, uint8_t* payload, uint8_t size);

/* ============================================================================================== */

#endif /* FRAMING_PROTOCOL_H */
