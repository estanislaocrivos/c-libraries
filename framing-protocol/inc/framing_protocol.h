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

#define MAX_PAYLOAD_SIZE    16
#define FRAME_OVERHEAD_SIZE 2 /* STX + ETX */
#define BCC_SIZE_BYTES      2
#define MAX_FRAME_SIZE      (MAX_PAYLOAD_SIZE + FRAME_OVERHEAD_SIZE + BCC_SIZE_BYTES)

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
    /**
     * @brief Pointer to the UART port structure. This structure must be first created and
     * initialized.
     */
    struct uart_port* _uart;

    /**
     * @brief Pointer to the Timer structure. This structure must be first created and
     * initialized.
     */
    struct timer* _timer;

    /**
     * @brief Byte used to store incoming bytes on the UART RX interrupt.
     */
    volatile uint8_t _rx_byte;

    /**
     * @brief RX buffer and index used for incoming bytes. This buffer is filled internally.
     */
    volatile uint8_t _rx_buffer[MAX_FRAME_SIZE];
    volatile uint8_t _rx_buffer_index;

    /**
     * @brief TX buffer and index used for outgoing bytes. This buffer is filled internally.
     */
    uint8_t _tx_buffer[MAX_FRAME_SIZE];
    uint8_t _tx_buffer_index;

    /**
     * @brief BCC buffer and index used for incoming BCC bytes. This buffer is filled internally.
     */
    uint8_t _bcc_buffer[BCC_SIZE_BYTES];
    uint8_t _bcc_buffer_index;

    /**
     * @brief Keeps track of whether the framing protocol instance has been initialized.
     */
    bool _initialized;

    /**
     * @brief Various internal state variables.
     */
    bool                 _waiting_for_ack;
    bool                 _receiving_frame;
    frame_parser_state_t _state;
    uint8_t              _payload_size;
    uint8_t              _lost_frames;
    uint8_t              _stx_byte;
    uint8_t              _etx_byte;
    uint8_t              _rx_timeout_ms;
    uint8_t              _tx_timeout_ms;
    uint8_t              _max_retransmissions;
    uint8_t              _retransmission_counter;

    /**
     * @brief Checksum calculator function pointer. This function must be provided by the user and
     * must match its signature. This function must calculate the checksum of a given buffer and
     * return it as an uint16_t.
     * @param buffer Pointer to the buffer for which the checksum is to be calculated.
     * @param size Size of the buffer in bytes.
     * @return Calculated checksum as an uint16_t.
     */
    uint16_t (*checksum_calculator)(const uint8_t* buffer, uint8_t size);

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

int8_t framing_protocol_receive_payload(framing_protocol_t* self, uint8_t* payload, uint8_t size);

/* ============================================================================================== */

#endif /* FRAMING_PROTOCOL_H */
