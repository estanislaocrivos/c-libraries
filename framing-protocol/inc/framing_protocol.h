#ifndef FRAMING_PROTOCOL_H
#define FRAMING_PROTOCOL_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ============================================================================================== */

#include "../../inc/errno.h"
#include "../../uart-interface/inc/uart.h"
#include "../../mcu-mngr-interface/inc/timer.h"

/* ============================================================================================== */

#define MAX_PAYLOAD_SIZE    16
#define FRAME_OVERHEAD_SIZE 2 /* STX + ETX */
#define BCC_SIZE_BYTES      2
#define MAX_FRAME_SIZE      (MAX_PAYLOAD_SIZE + FRAME_OVERHEAD_SIZE + BCC_SIZE_BYTES)

typedef enum
{
    STATE_PROCESS_STX,
    STATE_PROCESS_PAYLOAD,
    STATE_PROCESS_ETX,
    STATE_PROCESS_BCC,
    STATE_DISCARD_FRAME,
    STATE_FRAME_OK
} frame_parser_state_t;

typedef struct
{
    /**
     * @brief Pointer to the UART port structure. This structure must be first created and
     * initialized. The UART is expected to be initialized with the following parameters:
     * - Baud rate: to be defined by the user
     * - Data bits: 8 bits
     * - Parity: None
     * - Stop bits: 1 bit
     * - Flow control: None
     * - RX interrupt: enabled
     * - RX buffer size: 1 byte
     */
    struct uart_port* _uart;

    /**
     * @brief Pointer to the Timer structure. This structure must be first created and
     * initialized. The timer is expected to be initialized in timeout mode.
     */
    struct timer* _timer;

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
     * @brief Various internal state variables.
     */
    bool                 _initialized;
    bool                 _waiting_for_ack;
    bool                 _receiving_frame;
    frame_parser_state_t _state;

    /**
     * @brief Payload size in bytes. Must be defined by the user and must not exceed
     * MAX_PAYLOAD_SIZE.
     */
    uint8_t payload_size;

    /**
     * @brief Timeout between bytes during frame reception in milliseconds.
     */
    uint8_t rx_timeout_ms;

    /**
     * @brief Timeout to wait for an ACK after frame transmission in milliseconds.
     */
    uint8_t tx_timeout_ms;

    /**
     * @brief Maximum number of retransmissions before giving up.
     */
    uint8_t max_retransmissions;

    /**
     * @brief Counter for the number of transmissions (including retransmissions) of the current
     * frame.
     */
    uint8_t transmissions_counter;

    /**
     * @brief Counter for the number of lost frames detected due to errors.
     */
    uint8_t lost_frames_counter;

    /**
     * @brief Special bytes used in the framing protocol. If left null, default values will be used.
     */
    uint8_t _stx_byte;
    uint8_t _etx_byte;
    uint8_t _ack_byte;
    uint8_t _nack_byte;

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
