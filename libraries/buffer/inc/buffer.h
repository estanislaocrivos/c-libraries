#ifndef BUFFER_H
#define BUFFER_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * struct buffer - Linear byte buffer with index tracking
 * @buffer: Pointer to user-allocated storage
 * @size: Size of the buffer in bytes
 *
 * A simple linear buffer for sequential byte storage.
 * The user must allocate the backing storage and set public fields
 * before calling buffer_init().
 */
struct buffer
{
    /* public: user-configurable fields - set before init (const after init) */
    uint8_t* const buffer;
    const size_t   size;

    /* private: internal state - do not access directly */
    size_t index;
    bool   was_initialized;
};

/* ========================================================================== */

/**
 * @brief Initializes the buffer instance.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT if self or buffer is NULL, -EINVAL if size is
 * 0.
 */
int8_t buffer_init(struct buffer* self);

/* ========================================================================== */

/**
 * @brief Push a byte into the buffer at the current index.
 * @param self Pointer to the buffer instance.
 * @param byte Byte to push into the buffer.
 * @return 0 on success, -EFAULT if self is NULL, -EPERM if not initialized,
 * -ENOBUFS if full.
 */
int8_t buffer_push(struct buffer* self, uint8_t byte);

/* ========================================================================== */

/**
 * @brief Pop the last pushed byte from the buffer.
 * @param self Pointer to the buffer instance.
 * @param byte Pointer to store the popped byte.
 * @return 0 on success, -EFAULT if self or byte is NULL, -EPERM if not
 * initialized, -ENODATA if buffer is empty.
 */
int8_t buffer_pop(struct buffer* self, uint8_t* byte);

/* ========================================================================== */

/**
 * @brief Get a byte at a specific index without modifying the buffer state.
 * @param self Pointer to the buffer instance.
 * @param index Index of the byte to retrieve.
 * @param byte Pointer to store the retrieved byte.
 * @return 0 on success, -EFAULT if self or byte is NULL, -EPERM if not
 * initialized, -EINVAL if index is out of bounds.
 */
int8_t buffer_at(const struct buffer* self, size_t index, uint8_t* byte);

/* ========================================================================== */

/**
 * @brief Reset the buffer by clearing its contents and resetting the index.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT if self is NULL, -EPERM if not initialized.
 */
int8_t buffer_reset(struct buffer* self);

/* ========================================================================== */

/**
 * @brief Reset only the index to zero without clearing contents.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT if self is NULL, -EPERM if not initialized.
 */
int8_t buffer_reset_index(struct buffer* self);

/* ========================================================================== */

/**
 * @brief Get the current index (number of bytes written).
 * @param self Pointer to the buffer instance.
 * @param index Pointer to store the current index.
 * @return 0 on success, -EFAULT if self or index is NULL, -EPERM if not
 * initialized.
 */
int8_t buffer_get_index(const struct buffer* self, size_t* index);

/* ========================================================================== */

/**
 * @brief Get a pointer to the underlying raw buffer.
 * @param self Pointer to the buffer instance.
 * @param raw Pointer to store the raw buffer pointer.
 * @return 0 on success, -EFAULT if self or raw is NULL, -EPERM if not
 * initialized.
 */
int8_t buffer_get_raw(const struct buffer* self, uint8_t** raw);

/* ========================================================================== */

#endif /* BUFFER_H */
