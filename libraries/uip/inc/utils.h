#ifndef UTILS_H
#define UTILS_H

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

struct slice
{
    const uint8_t* base;
    uint16_t       len;
};

/* ========================================================================== */

/**
 * @brief Compute the Internet checksum (RFC 1071) over a byte buffer. The
 * checksum field within the buffer must be zeroed by the caller before
 * invoking this function.
 * @param frame_slice Array of struct slice containing each frame slice to be
 * evaluated as part of the checksum.
 * @param slice_count Number or slices contained in frame_slice array.
 * @return uint16_t One's complement of the one's complement sum of all 16-bit
 * words. Returns 0xFFFF if frame_size is 0.
 */
uint16_t compute_inet_checksum(
    const struct slice* frame_slice, uint8_t slice_count);

/* ========================================================================== */

#endif /* UTILS_H */
