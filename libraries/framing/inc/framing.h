#ifndef FRAMING_H
#define FRAMING_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

#include "../../libraries/crc/inc/crc.h"
#include "../../libraries/ring-buffer/inc/ring_buffer.h"
#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Frame parser states.
 */
enum framing_state
{
    FRAMING_START_STATE = 0,
    FRAMING_LENGTH_STATE,
    FRAMING_PAYLOAD_STATE,
    FRAMING_STOP_STATE,
    FRAMING_CRC_STATE,
    FRAMING_COMPLETE_STATE,
    FRAMING_ERROR_STATE
};

struct framing
{
    // Public
    struct crc*         crc_calculator;
    struct ring_buffer* rx_raw_buffer;
    struct ring_buffer* rx_payload_buffer;
    struct ring_buffer* tx_frame_buffer;
    uint8_t             start_delimiter;
    uint8_t             stop_delimiter;
    bool                use_crc8;
    bool                use_crc16;

    // Private
    uint8_t                 _payload_size;  // Stores the payload length byte retrieved from frame
    uint8_t*                _buffer;  // Internal buffer to build frames and parse incoming data
    size_t                  _buffer_size;    // Size of the internal buffer
    size_t                  _buffer_index;   // Current index in the internal buffer
    enum framing_state      _current_state;  // Current state of the framing state machine
    framing_state_handler_t _state_handler;  // Current state handler function pointer
    bool _was_initialized;  // Flag to indicate if the framing instance was initialized
};

/* ============================================================================================== */

int8_t framing_init(struct framing* self);

/* ============================================================================================== */

int8_t build_frame(struct framing* self, const uint8_t* payload, size_t payload_size);

/* ============================================================================================== */

int8_t retrieve_payload(struct framing* self, uint8_t* payload, size_t* payload_size);

/* ============================================================================================== */

#endif /* FRAMING_H */
