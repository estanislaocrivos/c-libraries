#include "ring_buffer.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

#define AVOID_MOD_OPERATION \
    1  // If true, uses if statements instead of modulo operations (avoids division, faster on
       // smaller MCUs)

/* ============================================================================================== */

int8_t initialize(struct ring_buffer* rb, struct ring_buffer_config* config)
{
    if (rb == NULL || config == NULL || config->buffer == NULL)
    {
        return -EFAULT;
    }
    if (config->size == 0)
    {
        return -EINVAL;
    }
    rb->_head   = 0;
    rb->_tail   = 0;
    rb->_config = config;
    memset(rb->_config->buffer, 0, config->size);
    rb->_was_initialized = true;
    return 0;
}

/* ============================================================================================== */

int8_t push(struct ring_buffer* self, const uint8_t* data, size_t len)
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
        if (next >= self->_config->size)
        {
            next = 0;
        }
#else
        next = (self->_head + 1) % self->_config->size;
#endif
        if (next == self->_tail)
        {
            if (!self->_config->overwrite)
            {
                return -ENOSPC;
            }
            else
            {
#if AVOID_MOD_OPERATION
                self->_tail += 1;
                if (self->_tail >= self->_config->size)
                {
                    self->_tail = 0;
                }
#else
                self->_tail = (self->_tail + 1) % self->_config->size;
#endif
            }
        }
        self->_config->buffer[self->_head] = data[count];
        self->_head                        = next;
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
        dest[count] = self->_config->buffer[self->_tail];
#if AVOID_MOD_OPERATION
        self->_tail += 1;
        if (self->_tail >= self->_config->size)
        {
            self->_tail = 0;
        }
#else
        self->_tail = (self->_tail + 1) % self->_config->size;
#endif
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
    if (!self->_was_initialized)
    {
        return -EPERM;
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
    if (!self->_was_initialized)
    {
        return -EPERM;
    }
#if AVOID_MOD_OPERATION
    size_t next_head = self->_head + 1;
    if (next_head >= self->_config->size)
    {
        next_head = 0;
    }
    *full = (next_head == self->_tail);
#else
    *full = ((self->_head + 1) % self->_config->size) == self->_tail;
#endif
    return 0;
}

/* ============================================================================================== */

int8_t available(const struct ring_buffer* self, size_t* available)
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
        used = self->_config->size - (self->_tail - self->_head);
    }
    *available = self->_config->size - used;
    return 0;
}

/* ============================================================================================== */
