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
    // Private
    volatile size_t _head;
    volatile size_t _tail;

    // Public
    uint8_t* buffer;
    size_t   size;
    bool     overwrite;
};

/* ============================================================================================== */

int8_t initialize(struct ring_buffer* rb, uint8_t* buffer, size_t size, bool overwrite);

/* ============================================================================================== */

size_t push(struct ring_buffer* self, const uint8_t* data, size_t len);

/* ============================================================================================== */

size_t pop(struct ring_buffer* self, uint8_t* dest, size_t len);

/* ============================================================================================== */

bool is_empty(const struct ring_buffer* self);

/* ============================================================================================== */

bool is_full(const struct ring_buffer* self);

/* ============================================================================================== */

size_t available(const struct ring_buffer* self);

/* ============================================================================================== */

#endif  // RING_BUFFER_H
