#ifndef UTILS_H
#define UTILS_H

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

/**
 * @brief Compute the Internet checksum (RFC 1071) over a byte buffer. The
 * checksum field within the buffer must be zeroed by the caller before
 * invoking this function.
 * @param frame Pointer to the buffer over which the checksum is computed.
 * @param frame_size Size of the buffer in bytes.
 * @return uint16_t One's complement of the one's complement sum of all 16-bit
 * words. Returns 0xFFFF if frame_size is 0.
 */
uint16_t compute_inet_checksum(const uint8_t* frame, uint16_t frame_size);

/* ========================================================================== */

#endif /* UTILS_H */
