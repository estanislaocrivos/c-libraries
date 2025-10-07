#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Unity is the test environment which includes Ceedling (framework for testing)
 * [https://www.throwtheswitch.org/ceedling]
 */
#include "/Library/Ruby/Gems/2.6.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"

/* Libraries to be tested. */
#include "../inc/core/serial_protocol/serial_protocol.h"
#include "../src/core/serial_protocol/serial_protocol.c"
#include "../src/core/serial_protocol/utilities.h"
#include "../src/core/serial_protocol/utilities.c"

/* Mocks */
#include "../build/test/mocks/mock_uart1_driver.h"
#include "../build/test/mocks/mock_tmr0_driver.h"

/* ============================================================================================== */

serial_protocol_t sp;

void test_serial_instance_create(void)
{
    sp = serial_protocol_create();
    TEST_ASSERT_NOT_NULL(&sp);
    TEST_ASSERT_EQUAL(serial_prot_initialize, sp.initialize);
    TEST_ASSERT_EQUAL(serial_prot_transmit_frame, sp.transmit_payload);
    TEST_ASSERT_EQUAL(serial_prot_retrieve_frame, sp.receive_payload);
    uart1_initialize_Expect();
    uart1_set_rx_callback_Expect(uart_rx_byte_callback);
    tmr0_initialize_Expect();
    tmr0_set_timeout_callback_Expect(tmr_timeout_callback);
    sp.initialize(&sp);
}

/* ============================================================================================== */

#define ERROR   0xCC
#define COMMAND 0xAA
#define ADDRESS 0xBB
#define DATA    0xABCD

uint8_t payload[FRAME_PAYLOAD_SIZE_BYTES]
    = {ERROR, COMMAND, ADDRESS, (uint8_t)(DATA >> 8), (uint8_t)(DATA & 0xFF)};

/* ============================================================================================== */

void test_rx_valid_frame(void)
{
    uint8_t frame[FULL_FRAME_SIZE_BYTES];
    build_frame(payload, FRAME_PAYLOAD_SIZE_BYTES, frame);
    printf("Valid frame: ");
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n");
    printf("Message length: %d\n", FULL_FRAME_SIZE_BYTES);

    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        tmr0_deactivate_timeout_Expect();
        tmr0_set_timeout_ms_Expect(RX_TIMEOUT_MS);
        uart_rx_byte_callback(frame[i]);
    }

    uart1_disable_rx_interrupt_Expect();
    tmr_timeout_callback();

    uart1_disable_rx_interrupt_Expect();
    uart1_transmit_byte_Expect(ACK);
    uart1_enable_rx_interrupt_Expect();
    TEST_ASSERT_EQUAL(FRAME_VALID, sp.receive_payload(&sp));
    TEST_ASSERT_EQUAL(sp.rx_payload[0], ERROR);
    TEST_ASSERT_EQUAL(sp.rx_payload[1], COMMAND);
    TEST_ASSERT_EQUAL(sp.rx_payload[2], ADDRESS);
    TEST_ASSERT_EQUAL(sp.rx_payload[3], (uint8_t)(DATA >> 8));
    TEST_ASSERT_EQUAL(sp.rx_payload[4], (uint8_t)(DATA & 0xFF));
}

/* ============================================================================================== */

void test_rx_invalid_frame(void)
{
    uint8_t frame[FULL_FRAME_SIZE_BYTES];
    build_frame(payload, FRAME_PAYLOAD_SIZE_BYTES, frame);
    frame[FULL_FRAME_SIZE_BYTES - 1] += 1; /* Make msg invalid */
    printf("Invalid frame: ");
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n");
    printf("Message length: %d\n", FULL_FRAME_SIZE_BYTES);

    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        tmr0_deactivate_timeout_Expect();
        tmr0_set_timeout_ms_Expect(RX_TIMEOUT_MS);
        uart_rx_byte_callback(frame[i]);
        TEST_ASSERT_EQUAL(frame[i], frame_parser.rx_buffer[i]);
    }
    uart1_disable_rx_interrupt_Expect();
    tmr_timeout_callback();

    uart1_disable_rx_interrupt_Expect();
    uart1_transmit_byte_Expect(NACK);
    uart1_enable_rx_interrupt_Expect();
    TEST_ASSERT_EQUAL(FRAME_INVALID, sp.receive_payload(&sp));
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[0], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[1], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[2], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[3], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[4], 0);
}

/* ============================================================================================== */

void test_rx_incomplete_frame(void)
{
    uint8_t frame[FULL_FRAME_SIZE_BYTES];
    build_frame(payload, FRAME_PAYLOAD_SIZE_BYTES, frame);
    printf("Valid frame: ");
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n");
    printf("Message length: %d\n", FULL_FRAME_SIZE_BYTES);

    /* Receive full frame but the last byte */
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES - 1; i++)
    {
        tmr0_deactivate_timeout_Expect();
        tmr0_set_timeout_ms_Expect(RX_TIMEOUT_MS);
        uart_rx_byte_callback(frame[i]);
        TEST_ASSERT_EQUAL(frame[i], frame_parser.rx_buffer[i]);
    }
    uart1_disable_rx_interrupt_Expect();
    tmr_timeout_callback();

    uart1_disable_rx_interrupt_Expect();
    uart1_transmit_byte_Expect(NACK);
    uart1_enable_rx_interrupt_Expect();
    TEST_ASSERT_EQUAL(FRAME_INVALID, sp.receive_payload(&sp));
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[0], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[1], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[2], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[3], 0);
    TEST_ASSERT_EQUAL(sp.rx_payload.payload[4], 0);
}

/* ============================================================================================== */

void test_tx_valid_frame(void)
{
    uint8_t frame[FULL_FRAME_SIZE_BYTES];
    build_frame(payload, FRAME_PAYLOAD_SIZE_BYTES, frame);
    printf("Valid frame: ");
    for (uint8_t i = 0; i < FULL_FRAME_SIZE_BYTES; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n");
    printf("Message length: %d\n", FULL_FRAME_SIZE_BYTES);
    memcpy(sp.tx_payload.payload, payload, FRAME_PAYLOAD_SIZE_BYTES);
    uart1_transmit_buffer_Expect(frame, FULL_FRAME_SIZE_BYTES);
    tmr0_set_timeout_ms_Expect(TX_TIMEOUT_MS);
    TEST_ASSERT_EQUAL(FRAME_TRANSMITTED, sp.transmit_payload(&sp));
}

/* ============================================================================================== */
