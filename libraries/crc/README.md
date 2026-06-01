# CRC Calculator

A simple C library to calculate CRC (Cyclic Redundancy Check) for data integrity verification.

## Usage Example

```c
#include "crc.h"

/* 1. Allocate variables and data */
uint8_t calculated_crc = 0;
uint8_t raw_data[] = {0x01, 0x02, 0x03, 0x04};

/* 2. Configure CRC-8 calculator */
struct crc crc8 = {
    .crc8_polynomial      = 0x97,
    .crc8_initial_value   = 0x00,
    .crc8_final_xor_value = 0x00,
    .reflect_input        = false,
    .reflect_output       = false
};

/* 3. Calculate CRC-8 for data */
crc8_calculate(&crc8, raw_data, sizeof(raw_data), &calculated_crc);
```
