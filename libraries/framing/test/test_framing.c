#include "../../test/support/unity.h"

/* ============================================================================================== */

#include <stdio.h>

#include "../inc/framing.h"
TEST_SOURCE_FILE("../src/framing.c")

#include "../../libraries/ring-buffer/inc/ring_buffer.h"
TEST_SOURCE_FILE("../../libraries/ring-buffer/src/ring_buffer.c")

#include "../../libraries/crc/inc/crc.h"
TEST_SOURCE_FILE("../../libraries/crc/src/crc.c")

/* ============================================================================================== */

void test_build_frame(void)
{
    uint8_t            _rx_raw_buffer[128]   = {0};
    uint8_t            _tx_frame_buffer[128] = {0};
    uint8_t            _internal_buffer[128] = {0};
    struct ring_buffer rx_buffer
        = {.buffer = _rx_raw_buffer, .size = sizeof(_rx_raw_buffer), .overwrite = false};
    ring_buffer_init(&rx_buffer);
    struct ring_buffer tx_buffer
        = {.buffer = _tx_frame_buffer, .size = sizeof(_tx_frame_buffer), .overwrite = false};
    ring_buffer_init(&tx_buffer);
    struct crc     crc8             = {.crc8_polynomial      = 0x97,
                                       .crc8_initial_value   = 0x00,
                                       .crc8_final_xor_value = 0x00,
                                       .reflect_input        = false,
                                       .reflect_output       = false};
    struct framing framing_instance = {.crc8_calculator      = &crc8,
                                       .rx_raw_buffer        = &rx_buffer,
                                       .tx_frame_buffer      = &tx_buffer,
                                       .internal_buffer      = _internal_buffer,
                                       .internal_buffer_size = sizeof(_internal_buffer),
                                       .start_delimiter      = 0xAA,
                                       .stop_delimiter       = 0x55};

    TEST_ASSERT_EQUAL(0, framing_init(&framing_instance));
    TEST_ASSERT_EQUAL(0, build_frame(&framing_instance, (uint8_t[]){0x01, 0x02}, 2));
    for (size_t i = 0; i < 6; i++)
    {
        uint8_t byte;
        TEST_ASSERT_EQUAL(0, pop(framing_instance.tx_frame_buffer, &byte, 1));
        printf("Byte %zu: 0x%02X\n", i, byte);
    }
}

/* ============================================================================================== */
