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

int8_t push(struct ring_buffer* self, const uint8_t* data, size_t len)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }
    if (len == 0)
    {
        return -EINVAL;
    }
    size_t count = 0;
    while (count < len)
    {
        size_t next = (self->_head + 1) % self->size;  // The same as doing if (next == size)
                                                       // {next = 0;} (avoids buffer overflow)
        if (next == self->_tail)
        {
            if (!self->overwrite)
            {
                return -ENOSPC;
            }
            else
            {
                self->_tail
                    = (self->_tail + 1) % self->size;  // The same asi doing if (tail == size) {tail
                                                       // = 0;} (avoids buffer overflow)
            }
        }
        self->buffer[self->_head] = data[count];
        self->_head               = next;
        count += 1;
    }
    return 0;
}

/* ============================================================================================== */

int8_t pop(struct ring_buffer* self, uint8_t* dest, size_t len)
{
    if (self == NULL || dest == NULL || len == 0)
    {
        return -EFAULT;
    }
    size_t count = 0;
    while (count < len && self->_tail != self->_head)
    {
        dest[count] = self->buffer[self->_tail];
        self->_tail = (self->_tail + 1) % self->size;
        count += 1;
    }
    return 0;
}

/* ============================================================================================== */

int8_t is_empty(const struct ring_buffer* self, bool* empty)
{
    if (self == NULL || empty == NULL)
    {
        return -EFAULT;
    }
    *empty = (self->_head == self->_tail);
    return 0;
}

/* ============================================================================================== */

int8_t is_full(const struct ring_buffer* self, bool* full)
{
    if (self == NULL || full == NULL)
    {
        return -EFAULT;
    }
    *full = ((self->_head + 1) % self->size) == self->_tail;
    return 0;
}

/* ============================================================================================== */

int8_t available(const struct ring_buffer* self, size_t* available)
{
    if (self == NULL || available == NULL)
    {
        return -EFAULT;
    }
    size_t used;
    if (self->_head >= self->_tail)
    {
        used = self->_head - self->_tail;
    }
    else
    {
        used = self->size - (self->_tail - self->_head);
    }
    *available = self->size - used;
    return 0;
}

/* ============================================================================================== */
