# Ring Buffer

A ring buffer (also known as a circular buffer) is a fixed-size data structure that uses a single, contiguous block of memory as if it were connected end-to-end. It is particularly useful for buffering data streams, such as audio or network packets, where the producer and consumer operate at different rates.

## Usage Example

```c
#include "ring_buffer.h"

#define BUFFER_SIZE 10

void main(void)
{
    /* 1. Create a raw buffer */
    uint8_t rx_raw_buffer[BUFFER_SIZE];

    /* 2. Initialize the ring buffer */
    struct ring_buffer rx_buffer
        = {.buffer = rx_raw_buffer, .size = sizeof(rx_raw_buffer), .overwrite = true};
    if (ring_buffer_init(&rx_buffer))
    {
        /* Handle error */
    }

    /* 3. Add data to the ring buffer and use! */
    uint8_t data_to_add[] = {1, 2, 3, 4, 5};
    ring_buffer_push(&rx_buffer, data_to_add, sizeof(data_to_add));
}
```
