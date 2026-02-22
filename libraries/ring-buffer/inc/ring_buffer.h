#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * struct ring_buffer - Circular buffer for byte streams
 * @buffer: Pointer to an already allocated buffer of size @size
 * @size: Size of the buffer array in bytes
 * @overwrite: If true, new data overwrites old data when buffer is full
 *
 * A circular buffer suitable for single-producer/single-consumer scenarios.
 * The user must allocate the backing storage and configure public fields
 * before calling ring_buffer_init().
 */
struct ring_buffer
{
    /* public: user-configurable fields - set before init (const after init) */
    uint8_t* const buffer;
    const size_t   size;
    const bool     overwrite;

    /* private: internal state - do not access directly */
    volatile size_t head;
    volatile size_t tail;
    bool            was_initialized;
};

/* ========================================================================== */

/**
 * @brief Initialize the ring buffer.
 * @param rb Pointer to the ring buffer instance with public fields configured.
 * @return 0 on success, -EFAULT if rb or buffer is NULL, -EINVAL if size is 0.
 */
int8_t ring_buffer_init(struct ring_buffer* rb);

/* ========================================================================== */

/**
 * @brief Push data into the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param data Pointer to the data to be pushed.
 * @param len Length of the data to be pushed.
 * @return 0 on success, -EFAULT if self or data is NULL, -EPERM if not
 * initialized, -EINVAL if len is 0, -ENOSPC if full and overwrite is disabled.
 */
int8_t ring_buffer_push(
    struct ring_buffer* self, const uint8_t* data, size_t len);

/* ========================================================================== */

/**
 * @brief Pop data from the ring buffer, removing it.
 * @param self Pointer to the ring buffer instance.
 * @param dest Pointer to the destination buffer.
 * @param len Number of bytes to pop.
 * @return 0 on success, -EFAULT if self or dest is NULL, -EPERM if not
 * initialized, -EINVAL if len is 0, -ENODATA if not enough data available.
 */
int8_t ring_buffer_pop(struct ring_buffer* self, uint8_t* dest, size_t len);

/* ========================================================================== */

/**
 * @brief Peek data from the ring buffer without removing it.
 * @param self Pointer to the ring buffer instance.
 * @param dest Pointer to the destination buffer.
 * @param len Number of bytes to peek.
 * @return 0 on success, -EFAULT if self or dest is NULL, -EPERM if not
 * initialized, -EINVAL if len is 0, -ENODATA if not enough data available.
 */
int8_t ring_buffer_peek(
    const struct ring_buffer* self, uint8_t* dest, size_t len);

/* ========================================================================== */

/**
 * @brief Check if the ring buffer is empty.
 * @param self Pointer to the ring buffer instance.
 * @param empty Pointer to store the result (true if empty).
 * @return 0 on success, -EFAULT if self or empty is NULL, -EPERM if not
 * initialized.
 */
int8_t ring_buffer_is_empty(const struct ring_buffer* self, bool* empty);

/* ========================================================================== */

/**
 * @brief Check if the ring buffer is full.
 * @param self Pointer to the ring buffer instance.
 * @param full Pointer to store the result (true if full).
 * @return 0 on success, -EFAULT if self or full is NULL, -EPERM if not
 * initialized.
 */
int8_t ring_buffer_is_full(const struct ring_buffer* self, bool* full);

/* ========================================================================== */

/**
 * @brief Get the number of available (free) bytes in the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param available Pointer to store the number of free bytes.
 * @return 0 on success, -EFAULT if self or available is NULL, -EPERM if not
 * initialized.
 */
int8_t ring_buffer_available(const struct ring_buffer* self, size_t* available);

/* ========================================================================== */

/**
 * @brief Get the number of used bytes in the ring buffer.
 * @param self Pointer to the ring buffer instance.
 * @param count Pointer to store the number of used bytes.
 * @return 0 on success, -EFAULT if self or count is NULL, -EPERM if not
 * initialized.
 */
int8_t ring_buffer_count(const struct ring_buffer* self, size_t* count);

/* ========================================================================== */

/**
 * @brief Reset the ring buffer, discarding all data.
 * @param self Pointer to the ring buffer instance.
 * @return 0 on success, -EFAULT if self is NULL, -EPERM if not initialized.
 */
int8_t ring_buffer_reset(struct ring_buffer* self);

/* ========================================================================== */

#endif  // RING_BUFFER_H
