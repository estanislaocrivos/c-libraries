#ifndef FRAMING_H
#define FRAMING_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../buffer/inc/buffer.h"
#include "../../crc/inc/crc.h"
#include "../../inc/errno.h"
#include "../../ring-buffer/inc/ring_buffer.h"

/* ========================================================================== */

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

/**
 * struct framing - Frame parser and builder for serial protocols
 * @crc8_calculator: Pointer to CRC calculator instance
 * @rx_raw_buffer: Ring buffer for incoming raw bytes (capacity >= 1 full frame)
 * @tx_frame_buffer: Linear buffer for built frames (capacity >= 1 full frame)
 * @parsing_buffer: Internal buffer for parsing (capacity >= 1 full frame)
 * @start_delimiter: Start delimiter byte value
 * @stop_delimiter: Stop delimiter byte value
 * @max_payload_size: Maximum allowed payload size
 * @lost_frames: Count of lost frames due to errors (read-only)
 *
 * State machine based frame parser supporting delimited frames with CRC-8.
 * Frame format: [START][LENGTH][PAYLOAD...][STOP][CRC8]
 * Configure public fields before calling framing_init().
 */
struct framing
{
    /* public: user-configurable fields - set before init (const after init) */
    struct crc* const         crc8_calculator;
    struct ring_buffer* const rx_raw_buffer;
    struct buffer* const      tx_frame_buffer;
    struct buffer* const      parsing_buffer;
    const uint8_t             start_delimiter;
    const uint8_t             stop_delimiter;
    const uint8_t             max_payload_size;

    /* public: read-only diagnostic field */
    uint8_t lost_frames;

    /* private: internal state - do not access directly */
    bool               frame_available;
    uint8_t            payload_size;
    enum framing_state current_state;
    bool               was_initialized;
};

/* ========================================================================== */

/**
 * @brief Initialize the framing instance.
 * @param self Pointer to the framing instance with public fields configured.
 * @return 0 on success, -EFAULT if self or any required pointer is NULL.
 */
int8_t framing_init(struct framing* self);

/* ========================================================================== */

/**
 * @brief Build a frame from the given payload into the TX frame buffer.
 * @param self Pointer to the framing instance.
 * @param payload Pointer to the payload data to be framed.
 * @param payload_size Size of the payload data.
 * @param frame_size Pointer to store the size of the built frame.
 * @return 0 on success, -EFAULT if any pointer is NULL, -EPERM if not
 * initialized, -ENOBUFS if frame exceeds buffer capacity.
 */
int8_t framing_build_frame(
    struct framing* self,
    const uint8_t*  payload,
    uint8_t         payload_size,
    uint8_t*        frame_size);

/* ========================================================================== */

/**
 * @brief Process one byte from the RX buffer through the frame parser state
 * machine.
 * @param self Pointer to the framing instance.
 * @return 0 if a complete frame was parsed (call framing_retrieve_payload
 * next), -EAGAIN if more data is needed, -ENODATA if RX buffer is empty,
 *         -EFAULT if self is NULL, -EPERM if not initialized.
 */
int8_t framing_process_incoming_data(struct framing* self);

/* ========================================================================== */

/**
 * @brief Retrieve payload after framing_process_incoming_data returns 0.
 * @param self Pointer to the framing instance.
 * @param payload Pointer to store the retrieved payload.
 * @param payload_size Pointer to store the size of the retrieved payload.
 * @return 0 on success, -EFAULT if any pointer is NULL, -EPERM if not
 * initialized, -ENODATA if no complete frame available, -EILSEQ if CRC error
 * (frame lost).
 */
int8_t framing_retrieve_payload(
    struct framing* self, uint8_t* payload, uint8_t* payload_size);

/* ========================================================================== */

#endif /* FRAMING_H */
