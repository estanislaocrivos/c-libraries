#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../../inc/errno.h"

/* ============================================================================================== */

struct ring_buffer
{
    /**
     * @brief Pointer to an already allocated buffer of size `size`.
     */
    uint8_t* buffer;

    /**
     * @brief Size of the buffer array. Must match the allocated buffer size.
     */
    size_t size;

    /**
     * @brief If true, new data will overwrite old data when the buffer is full.
     */
    bool overwrite;

    // Private
    volatile size_t _head;             // Head index
    volatile size_t _tail;             // Tail index
    bool            _was_initialized;  // Flag to indicate if the ring buffer was initialized
};

/* ============================================================================================== */

/**
 * @brief Initialize the ring buffer.
 * @param rb Pointer to an empty ring buffer instance.
 * @param config Pointer to the already initialized ring buffer configuration, must be valid during
 * the ring buffer's lifetime.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_init(struct ring_buffer* rb);

/* ============================================================================================== */

/**
 * @brief Push data into the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param data Pointer to the data to be pushed.
 * @param len Length of the data to be pushed.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_push(struct ring_buffer* self, const uint8_t* data, size_t len);

/* ============================================================================================== */

/**
 * @brief Pop data from the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param dest Pointer to the destination buffer where popped data will be stored.
 * @param len Length of the data to be popped. Must match (or be less than) the dest buffer size.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_pop(struct ring_buffer* self, uint8_t* dest, size_t len);

/* ============================================================================================== */

/**
 * @brief Check if the ring buffer is empty.
 * @param self Pointer to the ring buffer instance.
 * @param empty Pointer to a boolean that will be set to true if the buffer is empty, false
 * otherwise.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_is_empty(const struct ring_buffer* self, bool* empty);

/* ============================================================================================== */

/**
 * @brief Check if the ring buffer is full.
 * @param self Pointer to the ring buffer instance.
 * @param full Pointer to a boolean that will be set to true if the buffer is full, false
 * otherwise.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_is_full(const struct ring_buffer* self, bool* full);

/* ============================================================================================== */

/**
 * @brief Get the number of available bytes in the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param available Pointer to a size_t that will be set to the number of available bytes.
 * @return 0 on success, -ERRNO on failure.
 */
int8_t ring_buffer_available(const struct ring_buffer* self, size_t* available);

/* ============================================================================================== */

#endif  // RING_BUFFER_H
