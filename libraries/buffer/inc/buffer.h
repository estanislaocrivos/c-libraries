#ifndef BUFFER_H
#define BUFFER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

struct buffer
{
    uint8_t* buffer;
    size_t   size;
    size_t   index;
};

/* ============================================================================================== */

/**
 * @brief Initialize the buffer instance. This function checks for valid pointers and resets the
 * buffer.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT on invalid pointers or null size.
 */
int8_t buffer_init(struct buffer* self);

/* ============================================================================================== */

/**
 * @brief Push a byte into the buffer.
 * @param self Pointer to the buffer instance.
 * @param byte Byte to push into the buffer.
 * @return 0 on success, -ENOBUFS on failure due to buffer overflow, -EFAULT on invalid pointers.
 */
int8_t buffer_push(struct buffer* self, uint8_t byte);

/* ============================================================================================== */

/**
 * @brief Reset the buffer by clearing its contents and resetting the index.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT on invalid pointers.
 */
int8_t buffer_reset(struct buffer* self);

/* ============================================================================================== */

/**
 * @brief Reset only the index of the buffer to zero without clearing its contents.
 * @param self Pointer to the buffer instance.
 * @return 0 on success, -EFAULT on invalid pointers.
 */
int8_t buffer_reset_index(struct buffer* self);

/* ============================================================================================== */

#endif /* BUFFER_H */
