#include "ring_buffer.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

int8_t initialize(struct ring_buffer* rb, uint8_t* buffer, size_t size, bool overwrite)
{
    if (size == 0 || buffer == NULL)
    {
        return -EFAULT;
    }
    rb->buffer    = buffer;
    rb->size      = size;
    rb->_head     = 0;
    rb->_tail     = 0;
    rb->overwrite = overwrite;
    memset(rb->buffer, 0, size);
    return 0;
}

/* ============================================================================================== */

size_t push(struct ring_buffer* self, const uint8_t* data, size_t len)
{
    if (self == NULL || data == NULL || len == 0)
    {
        return 0;
    }
    size_t count = 0;
    while (count < len)
    {
        size_t next = (self->_head + 1) % self->size;
        if (next == self->_tail && !self->overwrite)
        {
            break;  // If full and not overwriting, stop
        }
        self->buffer[self->_head] = data[count];
        self->_head               = next;
        if (self->overwrite && self->_head == self->_tail)
        {
            self->_tail = (self->_tail + 1) % self->size;  // Move tail forward if overwriting
        }
        count += 1;
    }
    return count;
}

/* ============================================================================================== */

size_t pop(struct ring_buffer* self, uint8_t* dest, size_t len)
{
    if (self == NULL || dest == NULL || len == 0)
    {
        return 0;
    }
    size_t count = 0;
    while (count < len && self->_tail != self->_head)
    {
        dest[count] = self->buffer[self->_tail];
        self->_tail = (self->_tail + 1) % self->size;
        count += 1;
    }
    return count;
}

/* ============================================================================================== */

bool is_empty(const struct ring_buffer* self)
{
    return self->_head == self->_tail;
}

/* ============================================================================================== */

bool is_full(const struct ring_buffer* self)
{
    return ((self->_head + 1) % self->size) == self->_tail;
}

/* ============================================================================================== */

size_t available(const struct ring_buffer* self)
{
    size_t used;
    if (self->_head >= self->_tail)
    {
        used = self->_head - self->_tail;
    }
    else
    {
        used = self->size - (self->_tail - self->_head);
    }
    return self->size - used;
}

/* ============================================================================================== */
