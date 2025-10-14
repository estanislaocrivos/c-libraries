#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../../inc/errno.h"

/* ============================================================================================== */

struct ring_buffer_config
{
    // Public
    uint8_t* buffer;    /**< Pointer to the buffer array */
    size_t   size;      /**< Size of the buffer array */
    bool     overwrite; /**< Overwrite old data when full if true */
};

struct ring_buffer
{
    // Private
    volatile size_t _head; /**< Head index */
    volatile size_t _tail; /**< Tail index */

    // Public
    struct ring_buffer_config* config; /**< Configuration struct */
};

/* ============================================================================================== */

int8_t initialize(struct ring_buffer* rb, struct ring_buffer_config* config);

/* ============================================================================================== */

int8_t push(struct ring_buffer* self, const uint8_t* data, size_t len);

/* ============================================================================================== */

int8_t pop(struct ring_buffer* self, uint8_t* dest, size_t len);

/* ============================================================================================== */

int8_t is_empty(const struct ring_buffer* self, bool* empty);

/* ============================================================================================== */

int8_t is_full(const struct ring_buffer* self, bool* full);

/* ============================================================================================== */

int8_t available(const struct ring_buffer* self, size_t* available);

/* ============================================================================================== */

#endif  // RING_BUFFER_H
