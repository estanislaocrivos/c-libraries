# CRC Calculator ☎️

A simple C library to calculate CRC (Cyclic Redundancy Check) for data integrity verification.

## Features

- Supports CRC-8 for now.
- Easy to integrate into C projects.
- Tested for multiple polynomials and data sets.

## Usage

1. Include the header file in your project:

   ```c
   #include "crc.h"
   ```

2. Create a CRC object, configure it, and compute the CRC for your data:

   ```c
   const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0x00,
                             .crc8_polynomial      = 0x9B,
                             .reflect_input        = false,
                             .reflect_output       = false};

    const uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data, sizeof(data), &result);
   ```

## What is CRC?

Cyclic Redundancy Check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data. It is based on polynomial division and provides a way to verify the integrity of data. Each CRC algorithm is defined by its polynomial, initial value, final XOR value, and whether the input and output are reflected. A CRC value is computed by performing binary division of the data by a predetermined polynomial, and the remainder of this division is the CRC value. Each polynomial offers different levels of error detection capabilities (see [CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) for more details, use [CRC CALC](https://crccalc.com/?crc=123456789&method=&datatype=ascii&outtype=hex) to play around).

## What this project taught me

- Understanding of CRC algorithms and their implementation.
- Bitwise operations and optimizations in C.
- Writing modular and reusable C code.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
