#include "../inc/buffer.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

int8_t buffer_init(struct buffer* self)
{
    if (self == NULL || self->buffer == NULL || self->size == 0)
    {
        return -EFAULT;
    }
    self->index = 0;
    memset(self->buffer, 0, self->size);
    return 0;
}

/* ============================================================================================== */

int8_t buffer_push(struct buffer* self, uint8_t byte)
{
    if (self == NULL || self->buffer == NULL)
    {
        return -EFAULT;
    }
    if (self->index >= self->size)
    {
        return -ENOBUFS;
    }
    self->buffer[self->index] = byte;
    self->index += 1;
    return 0;
}

/* ============================================================================================== */

int8_t buffer_reset(struct buffer* self)
{
    if (self == NULL || self->buffer == NULL)
    {
        return -EFAULT;
    }
    memset(self->buffer, 0, self->size);
    self->index = 0;
    return 0;
}

/* ============================================================================================== */

int8_t buffer_reset_index(struct buffer* self)
{
    if (self == NULL || self->buffer == NULL)
    {
        return -EFAULT;
    }
    self->index = 0;
    return 0;
}

/* ============================================================================================== */
