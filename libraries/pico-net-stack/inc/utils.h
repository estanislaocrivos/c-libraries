#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint16_t compute_inet_checksum(const uint8_t* frame, const uint16_t frame_size);

#endif
