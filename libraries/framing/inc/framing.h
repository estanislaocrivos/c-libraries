#ifndef FRAMING_H
#define FRAMING_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../libraries/crc/inc/crc.h"
#include "../../libraries/ring-buffer/inc/ring_buffer.h"
#include "../../libraries/buffer/inc/buffer.h"
#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Frame parser internal states.
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
    /**
     * @brief Pointer to the CRC calculator instance.
     */
    struct crc* crc8_calculator;

    /**
     * @brief Pointer to the RX raw data ring buffer. Each byte received through the communication
     * interface should be pushed into this buffer. Must have capacity for at least one full frame.
     */
    struct ring_buffer* rx_raw_buffer;

    /**
     * @brief Pointer to the TX frame linear buffer. Built frame will be pushed into this
     * buffer for transmission. Must have capacity for at least one full frame.
     */
    struct buffer* tx_frame_buffer;

    /**
     * @brief Internal buffer to build frames and parse incoming data. The size must match the
     * maximum expected frame size.
     */
    struct buffer* parsing_buffer;

    /**
     * @brief Start delimiter byte value.
     */
    uint8_t start_delimiter;

    /**
     * @brief Stop delimiter byte value.
     */
    uint8_t stop_delimiter;

    // Private
    uint8_t            _payload_size;   // Stores the payload length byte retrieved from frame
    enum framing_state _current_state;  // Current state of the framing state machine
    bool _was_initialized;              // Flag to indicate if the framing instance was initialized
};

/* ============================================================================================== */

/**
 * @brief Initialize the framing instance.
 * @param self Pointer to the framing instance.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t framing_init(struct framing* self);

/* ============================================================================================== */

/**
 * @brief Build a frame from the given payload and push it into the TX frame buffer.
 * @param self Pointer to the framing instance.
 * @param payload Pointer to the payload data to be framed.
 * @param payload_size Size of the payload data.
 * @param frame_size Pointer to store the size of the built frame.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t build_frame(struct framing* self,
                   const uint8_t*  payload,
                   uint8_t         payload_size,
                   uint8_t*        frame_size);

/* ============================================================================================== */

/**
 * @brief Process incoming data from the RX raw data buffer. Must be called repeatedly to parse
 * incoming bytes.
 * @param self Pointer to the framing instance.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t process_incoming_data(struct framing* self);

/* ============================================================================================== */

/**
 * @brief Retrieve a complete payload from the RX raw data buffer if available. Must be called after
 * `process_incoming_data` returns successfully (0), indicating that a full frame has been
 * processed.
 * @param self Pointer to the framing instance.
 * @param payload Pointer to store the retrieved payload.
 * @param payload_size Pointer to store the size of the retrieved payload.
 * @return 0 on success, -ENODATA if no complete frame is available, -EILSEQ if a framing error
 * occurred, -ERRNO on other failures.
 */
int8_t retrieve_payload(struct framing* self, uint8_t* payload, uint8_t* payload_size);

/* ============================================================================================== */

#endif /* FRAMING_H */
