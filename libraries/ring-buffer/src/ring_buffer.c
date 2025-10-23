#include "../inc/ring_buffer.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

#define AVOID_MOD_OPERATION \
    1  // If true, uses if statements instead of modulo operations (avoids division, faster on
       // smaller MCUs)

/* ============================================================================================== */

int8_t ring_buffer_init(struct ring_buffer* rb)
{
    if (rb == NULL || rb->buffer == NULL)
    {
        return -EFAULT;
    }
    if (rb->size == 0)
    {
        return -EINVAL;
    }
    rb->_head = 0;
    rb->_tail = 0;
    memset(rb->buffer, 0, rb->size);
    rb->_was_initialized = true;
    return 0;
}

/* ============================================================================================== */

int8_t ring_buffer_push(struct ring_buffer* self, const uint8_t* data, size_t len)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
    if (len == 0)
    {
        return -EINVAL;
    }
    size_t count = 0;
    while (count < len)
    {
        size_t next;
#if AVOID_MOD_OPERATION
        next = self->_head + 1;
        if (next >= self->size)
        {
            next = 0;
        }
#else
        next = (self->_head + 1) % self->size;
#endif
        if (next == self->_tail)
        {
            if (!self->overwrite)
            {
                return -ENOSPC;
            }
            else
            {
#if AVOID_MOD_OPERATION
                self->_tail += 1;
                if (self->_tail >= self->size)
                {
                    self->_tail = 0;
                }
#else
                self->_tail = (self->_tail + 1) % self->size;
#endif
            }
        }
        self->buffer[self->_head] = data[count];
        self->_head               = next;
        count += 1;
    }
    return 0;
}

/* ============================================================================================== */

int8_t ring_buffer_pop(struct ring_buffer* self, uint8_t* dest, size_t len)
{
    if (self == NULL || dest == NULL || len == 0)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
    size_t count = 0;
    while (count < len)
    {
        if (self->_tail == self->_head)
        {
            return -ENODATA;
        }
        dest[count] = self->buffer[self->_tail];
#if AVOID_MOD_OPERATION
        self->_tail += 1;
        if (self->_tail >= self->size)
        {
            self->_tail = 0;
        }
#else
        self->_tail = (self->_tail + 1) % self->size;
#endif
        count += 1;
    }
    return 0;
}

/* ============================================================================================== */

int8_t ring_buffer_is_empty(const struct ring_buffer* self, bool* empty)
{
    if (self == NULL || empty == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
    *empty = (self->_head == self->_tail);
    return 0;
}

/* ============================================================================================== */

int8_t ring_buffer_is_full(const struct ring_buffer* self, bool* full)
{
    if (self == NULL || full == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
#if AVOID_MOD_OPERATION
    size_t next_head = self->_head + 1;
    if (next_head >= self->size)
    {
        next_head = 0;
    }
    *full = (next_head == self->_tail);
#else
    *full = ((self->_head + 1) % self->size) == self->_tail;
#endif
    return 0;
}

/* ============================================================================================== */

int8_t ring_buffer_available(const struct ring_buffer* self, size_t* available)
{
    if (self == NULL || available == NULL)
    {
        return -EFAULT;
    }
    if (!self->_was_initialized)
    {
        return -EPERM;
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
