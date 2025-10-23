# Ring Buffer 💍

A ring buffer (also known as a circular buffer) is a fixed-size data structure that uses a single, contiguous block of memory as if it were connected end-to-end. It is particularly useful for buffering data streams, such as audio or network packets, where the producer and consumer operate at different rates.

## Usage

```c
#include "ring_buffer.h"

void main(void)
{
    uint8_t                   buffer[10] = {0};
    struct ring_buffer        rb         = {0};
    struct ring_buffer_config config     = {
            .buffer    = buffer,
            .size      = sizeof(buffer),
            .overwrite = true,
    };
    if (initialize(&rb, &config))
    {
        // Handle error
    }
    const uint8_t data_to_push[] = {1, 2, 3, 4, 5};
    if (ring_buffer_push(&rb, data_to_push, sizeof(data_to_push)))
    {
        // Handle error
    }
    uint8_t popped_data[5] = {0};
    if (ring_buffer_pop(&rb, popped_data, sizeof(popped_data)))
    {
        // Handle error
    }
}
```
