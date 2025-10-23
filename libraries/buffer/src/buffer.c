#include "../inc/buffer.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

int8_t buffer_init(struct buffer* buf)
{
    if (buf == NULL || buf->buffer == NULL || buf->size == 0)
    {
        return -EFAULT;
    }
    buf->index = 0;
    memset(buf->buffer, 0, buf->size);
    return 0;
}

/* ============================================================================================== */

int8_t buffer_push(struct buffer* buf, uint8_t byte)
{
    if (buf == NULL || buf->buffer == NULL)
    {
        return -EFAULT;
    }
    if (buf->index >= buf->size)
    {
        return -ENOBUFS;
    }
    buf->buffer[buf->index] = byte;
    buf->index += 1;
    return 0;
}

/* ============================================================================================== */

int8_t buffer_clear(struct buffer* buf)
{
    if (buf == NULL || buf->buffer == NULL)
    {
        return -EFAULT;
    }
    memset(buf->buffer, 0, buf->size);
    buf->index = 0;
    return 0;
}

/* ============================================================================================== */

int8_t buffer_reset_index(struct buffer* buf)
{
    if (buf == NULL || buf->buffer == NULL)
    {
        return -EFAULT;
    }
    buf->index = 0;
    return 0;
}

/* ============================================================================================== */
