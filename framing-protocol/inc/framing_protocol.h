#ifndef FRAMING_PROTOCOL_H
#define FRAMING_PROTOCOL_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ============================================================================================== */

#include "../../uart-interface/inc/uart.h"
#include "../../mcu-mngr-interface/inc/mcu_manager.h"

/* ============================================================================================== */

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
    uart_t*            _uart;
    timer_interface_t* _timer;

    uint8_t* rx_buffer;
    uint8_t* tx_buffer;
    uint8_t  buffer_size;

    frame_parser_state_t _state;
    bool                 receiving_frame;
    uint8_t              lost_frames;
    uint8_t              stx_byte;
    uint8_t              etx_byte;

    void (*on_frame_received)(uint8_t* data, uint8_t length);  // Callback
    void (*on_frame_error)(void);                              // Callback opcional

} framing_protocol_t;

/* ============================================================================================== */

int8_t framing_protocol_create(framing_protocol_t* self);

/* ============================================================================================== */

#endif /* FRAMING_PROTOCOL_H */
