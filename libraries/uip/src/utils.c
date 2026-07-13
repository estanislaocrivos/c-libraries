#include "../inc/utils.h"

#include <stdint.h>

/* ========================================================================== */

uint16_t compute_inet_checksum(
    const struct slice* frame_slice, uint8_t slice_count)
{
    uint32_t acc = 0;

    for (uint8_t slice_idx = 0; slice_idx < slice_count; slice_idx += 1)
    {
        const uint8_t* local_slice     = frame_slice[slice_idx].base;
        uint16_t       local_slice_len = frame_slice[slice_idx].len;

        /* Process frame_slice[slice].base in 16-bit blocks */
        while (local_slice_len > 1)
        {
            acc += ((uint32_t)local_slice[0] << 8) | local_slice[1];
            local_slice += 2;
            local_slice_len -= 2;
        }

        /* Handle an odd byte if the frame_slice[slice].base
         * frame_slice[slice].len is not even */
        if (local_slice_len == 1)
        {
            acc += ((uint32_t)local_slice[0] << 8);
        }

        /* Fold 32-bit acc into 16 bits by adding carry-outs until no carries
         * remain
         */
        while (acc >> 16)
        {
            acc = (acc & 0xFFFF) + (acc >> 16);
        }
    }

    /* Return the final one's complement */
    return (uint16_t)(~acc);
}

/* ========================================================================== */
