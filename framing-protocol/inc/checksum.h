#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "stdint.h"

uint16_t basic_checksum(const uint8_t* buffer, uint8_t length);

uint16_t crc16_checksum(const uint8_t* buffer, uint8_t length);

#endif  // CHECKSUM_H
