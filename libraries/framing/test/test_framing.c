#include "../../test/support/unity.h"

/* ============================================================================================== */

#include <stdio.h>

#include "../inc/framing.h"
TEST_SOURCE_FILE("../src/framing.c")

#include "../../ring-buffer/inc/ring_buffer.h"
TEST_SOURCE_FILE("../../ring-buffer/src/ring_buffer.c")

#include "../../buffer/inc/buffer.h"
TEST_SOURCE_FILE("../../buffer/src/buffer.c")

#include "../../crc/inc/crc.h"
TEST_SOURCE_FILE("../../crc/src/crc.c")

/* ============================================================================================== */

void test_build_frame(void)
{
    uint8_t _rx_raw_buffer[128]   = {0};
    uint8_t _tx_frame_buffer[128] = {0};
    uint8_t _internal_buffer[128] = {0};

    struct ring_buffer rx_buffer
        = {.buffer = _rx_raw_buffer, .size = sizeof(_rx_raw_buffer), .overwrite = false};
    ring_buffer_init(&rx_buffer);

    struct buffer tx_buffer
        = {.buffer = _tx_frame_buffer, .size = sizeof(_tx_frame_buffer), .index = 0};
    buffer_init(&tx_buffer);

    struct buffer int_buffer
        = {.buffer = _internal_buffer, .size = sizeof(_internal_buffer), .index = 0};
    buffer_init(&int_buffer);

    struct crc crc8 = {.crc8_polynomial      = 0x97,
                       .crc8_initial_value   = 0x00,
                       .crc8_final_xor_value = 0x00,
                       .reflect_input        = false,
                       .reflect_output       = false};

    struct framing framing_instance = {.crc8_calculator = &crc8,
                                       .rx_raw_buffer   = &rx_buffer,
                                       .tx_frame_buffer = &tx_buffer,
                                       .parsing_buffer  = &int_buffer,
                                       .start_delimiter = 0xAA,
                                       .stop_delimiter  = 0x55};

    TEST_ASSERT_EQUAL(0, framing_init(&framing_instance));
    const uint8_t payload[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t       frame_size;
    TEST_ASSERT_EQUAL(0, build_frame(&framing_instance, payload, sizeof(payload), &frame_size));
    for (size_t i = 0; i < frame_size; i++)
    {
        printf("Frame byte %zu: 0x%02X\n", i, framing_instance.tx_frame_buffer->buffer[i]);
    }
}

/* ============================================================================================== */

// void test_retrieve_payload_no_error(void)
// {
//     uint8_t            _rx_raw_buffer[128]   = {0};
//     uint8_t            _tx_frame_buffer[128] = {0};
//     uint8_t            _internal_buffer[128] = {0};
//     struct ring_buffer rx_buffer
//         = {.buffer = _rx_raw_buffer, .size = sizeof(_rx_raw_buffer), .overwrite = false};
//     ring_buffer_init(&rx_buffer);
//     struct ring_buffer tx_buffer
//         = {.buffer = _tx_frame_buffer, .size = sizeof(_tx_frame_buffer), .overwrite = false};
//     ring_buffer_init(&tx_buffer);
//     struct crc     crc8             = {.crc8_polynomial      = 0x97,
//                                        .crc8_initial_value   = 0x00,
//                                        .crc8_final_xor_value = 0x00,
//                                        .reflect_input        = false,
//                                        .reflect_output       = false};
//     struct framing framing_instance = {.crc8_calculator      = &crc8,
//                                        .rx_raw_buffer        = &rx_buffer,
//                                        .tx_frame_buffer      = &tx_buffer,
//                                        .internal_buffer      = _internal_buffer,
//                                        .internal_buffer_size = sizeof(_internal_buffer),
//                                        .start_delimiter      = 0xAA,
//                                        .stop_delimiter       = 0x55};

//     TEST_ASSERT_EQUAL(0, framing_init(&framing_instance));

//     const uint8_t frame[] = {0xAA, 0x04, 0x01, 0x02, 0x03, 0x04, 0x55, 0x33};
//     push(framing_instance.rx_raw_buffer, frame, sizeof(frame));

//     uint8_t payload[64]  = {0};
//     uint8_t payload_size = 0;

//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(0, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(0, retrieve_payload(&framing_instance, payload, &payload_size));

//     for (size_t i = 0; i < payload_size; i++)
//     {
//         printf("Payload byte %zu: 0x%02X\n", i, payload[i]);
//     }
// }

// /* ==============================================================================================
// */

// void test_retrieve_payload_error(void)
// {
//     uint8_t            _rx_raw_buffer[128]   = {0};
//     uint8_t            _tx_frame_buffer[128] = {0};
//     uint8_t            _internal_buffer[128] = {0};
//     struct ring_buffer rx_buffer
//         = {.buffer = _rx_raw_buffer, .size = sizeof(_rx_raw_buffer), .overwrite = false};
//     ring_buffer_init(&rx_buffer);
//     struct ring_buffer tx_buffer
//         = {.buffer = _tx_frame_buffer, .size = sizeof(_tx_frame_buffer), .overwrite = false};
//     ring_buffer_init(&tx_buffer);
//     struct crc     crc8             = {.crc8_polynomial      = 0x97,
//                                        .crc8_initial_value   = 0x00,
//                                        .crc8_final_xor_value = 0x00,
//                                        .reflect_input        = false,
//                                        .reflect_output       = false};
//     struct framing framing_instance = {.crc8_calculator      = &crc8,
//                                        .rx_raw_buffer        = &rx_buffer,
//                                        .tx_frame_buffer      = &tx_buffer,
//                                        .internal_buffer      = _internal_buffer,
//                                        .internal_buffer_size = sizeof(_internal_buffer),
//                                        .start_delimiter      = 0xAA,
//                                        .stop_delimiter       = 0x55};

//     TEST_ASSERT_EQUAL(0, framing_init(&framing_instance));

//     const uint8_t frame[] = {0xAA, 0x04, 0x01, 0x02, 0x03, 0x04, 0x55, 0x34};  // Incorrect CRC
//     push(framing_instance.rx_raw_buffer, frame, sizeof(frame));

//     uint8_t payload[64]  = {0};
//     uint8_t payload_size = 0;
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EAGAIN, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(0, process_incoming_data(&framing_instance));
//     TEST_ASSERT_EQUAL(-EILSEQ, retrieve_payload(&framing_instance, payload, &payload_size));

//     for (size_t i = 0; i < payload_size; i++)
//     {
//         printf("Payload byte %zu: 0x%02X\n", i, payload[i]);
//     }
// }

/* ============================================================================================== */
