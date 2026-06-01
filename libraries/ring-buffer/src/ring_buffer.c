#include "../inc/ring_buffer.h"

/* ========================================================================== */

#include <string.h>

/* ========================================================================== */

#define AVOID_MOD_OPERATION \
    1  // If true, uses if statements instead of modulo operations (avoids
       // division, faster on smaller MCUs)

/* ========================================================================== */

/* PUBLIC */

/* ========================================================================== */

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
    rb->head = 0;
    rb->tail = 0;
    memset(rb->buffer, 0, rb->size);
    rb->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_push(
    struct ring_buffer* self, const uint8_t* data, size_t len)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
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
        next = self->head + 1;
        if (next >= self->size)
        {
            next = 0;
        }
#else
        next = (self->head + 1) % self->size;
#endif
        if (next == self->tail)
        {
            if (!self->overwrite)
            {
                return -ENOSPC;
            }
            else
            {
#if AVOID_MOD_OPERATION
                self->tail += 1;
                if (self->tail >= self->size)
                {
                    self->tail = 0;
                }
#else
                self->tail = (self->tail + 1) % self->size;
#endif
            }
        }
        self->buffer[self->head] = data[count];
        self->head               = next;
        count += 1;
    }
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_pop(struct ring_buffer* self, uint8_t* dest, size_t len)
{
    if (self == NULL || dest == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
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
        if (self->tail == self->head)
        {
            return -ENODATA;
        }
        dest[count] = self->buffer[self->tail];
#if AVOID_MOD_OPERATION
        self->tail += 1;
        if (self->tail >= self->size)
        {
            self->tail = 0;
        }
#else
        self->tail = (self->tail + 1) % self->size;
#endif
        count += 1;
    }
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_is_empty(const struct ring_buffer* self, bool* empty)
{
    if (self == NULL || empty == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    *empty = (self->head == self->tail);
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_is_full(const struct ring_buffer* self, bool* full)
{
    if (self == NULL || full == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
#if AVOID_MOD_OPERATION
    size_t nexthead = self->head + 1;
    if (nexthead >= self->size)
    {
        nexthead = 0;
    }
    *full = (nexthead == self->tail);
#else
    *full = ((self->head + 1) % self->size) == self->tail;
#endif
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_available(const struct ring_buffer* self, size_t* available)
{
    if (self == NULL || available == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    size_t used;
    if (self->head >= self->tail)
    {
        used = self->head - self->tail;
    }
    else
    {
        used = self->size - (self->tail - self->head);
    }
    *available = self->size - used;
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_count(const struct ring_buffer* self, size_t* count)
{
    if (self == NULL || count == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (self->head >= self->tail)
    {
        *count = self->head - self->tail;
    }
    else
    {
        *count = self->size - (self->tail - self->head);
    }
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_peek(
    const struct ring_buffer* self, uint8_t* dest, size_t len)
{
    if (self == NULL || dest == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (len == 0)
    {
        return -EINVAL;
    }
    size_t count     = 0;
    size_t temp_tail = self->tail;
    while (count < len)
    {
        if (temp_tail == self->head)
        {
            return -ENODATA;
        }
        dest[count] = self->buffer[temp_tail];
#if AVOID_MOD_OPERATION
        temp_tail += 1;
        if (temp_tail >= self->size)
        {
            temp_tail = 0;
        }
#else
        temp_tail = (temp_tail + 1) % self->size;
#endif
        count += 1;
    }
    return 0;
}

/* ========================================================================== */

int8_t ring_buffer_reset(struct ring_buffer* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    self->head = 0;
    self->tail = 0;
    return 0;
}

/* ========================================================================== */
