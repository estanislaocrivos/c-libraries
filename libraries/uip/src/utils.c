#include "../inc/utils.h"

#include <stdint.h>

uint16_t compute_inet_checksum(const uint8_t* frame, uint16_t frame_size)
{
    uint32_t acc = 0;

    /* Process frame in 16-bit blocks */
    while (frame_size > 1)
    {
        acc += ((uint32_t)frame[0] << 8) | frame[1];
        frame += 2;
        frame_size -= 2;
    }

    /* Handle an odd byte if the frame frame_size is not even */
    if (frame_size == 1)
    {
        acc += ((uint32_t)frame[0] << 8);
    }

    /* Fold 32-bit acc into 16 bits by adding carry-outs until no carries remain
     */
    while (acc >> 16)
    {
        acc = (acc & 0xFFFF) + (acc >> 16);
    }

    /* Return the final one's complement */
    return (uint16_t)(~acc);
}
