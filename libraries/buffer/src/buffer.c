#include "../inc/buffer.h"

/* ========================================================================== */

#include <string.h>

/* ========================================================================== */

/* PUBLIC */

/* ========================================================================== */

int8_t buffer_init(struct buffer* self)
{
    if (self == NULL || self->buffer == NULL)
    {
        return -EFAULT;
    }
    if (self->size == 0)
    {
        return -EINVAL;
    }
    self->index = 0;
    memset(self->buffer, 0, self->size);
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t buffer_push(struct buffer* self, uint8_t byte)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (self->index >= self->size)
    {
        return -ENOBUFS;
    }
    self->buffer[self->index] = byte;
    self->index += 1;
    return 0;
}

/* ========================================================================== */

int8_t buffer_pop(struct buffer* self, uint8_t* byte)
{
    if (self == NULL || byte == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (self->index == 0)
    {
        return -ENODATA;
    }
    self->index -= 1;
    *byte = self->buffer[self->index];
    return 0;
}

/* ========================================================================== */

int8_t buffer_at(const struct buffer* self, size_t index, uint8_t* byte)
{
    if (self == NULL || byte == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (index >= self->size)
    {
        return -EINVAL;
    }
    *byte = self->buffer[index];
    return 0;
}

/* ========================================================================== */

int8_t buffer_reset(struct buffer* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    memset(self->buffer, 0, self->size);
    self->index = 0;
    return 0;
}

/* ========================================================================== */

int8_t buffer_reset_index(struct buffer* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    self->index = 0;
    return 0;
}

/* ========================================================================== */

int8_t buffer_get_index(const struct buffer* self, size_t* index)
{
    if (self == NULL || index == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    *index = self->index;
    return 0;
}

/* ========================================================================== */

int8_t buffer_get_raw(const struct buffer* self, uint8_t** raw)
{
    if (self == NULL || raw == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    *raw = self->buffer;
    return 0;
}

/* ========================================================================== */
