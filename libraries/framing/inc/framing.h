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
    FRAMING_STATE_IDLE,
    FRAMING_STATE_START,
    FRAMING_STATE_LENGTH,
    FRAMING_STATE_PAYLOAD,
    FRAMING_STATE_CRC,
    FRAMING_STATE_STOP,
    FRAMING_STATE_COMPLETE,
    FRAMING_STATE_ERROR
};

struct framing
{
    struct crc*         crc_calculator;
    struct ring_buffer* rx_ring_buffer;
    uint8_t*            rx_payload_buffer;
    uint8_t*            tx_frame_buffer;
    uint8_t             start_delimiter;
    uint8_t             stop_delimiter;
    bool                use_crc8;
    bool                use_crc16;
};

/* ============================================================================================== */

int8_t framing_init(struct framing* framing_instance);

/* ============================================================================================== */

int8_t build_frame(struct framing* framing_instance, const uint8_t* payload, size_t payload_size);

/* ============================================================================================== */

int8_t retrieve_payload(struct framing* framing_instance,
                        uint8_t*        payload_buffer,
                        size_t*         payload_size);

/* ============================================================================================== */

#endif /* FRAMING_H */
