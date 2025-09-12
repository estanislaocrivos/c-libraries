#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/uart.h"
#include "../src/uart.c"

/* Mocked interfaces */
#include "mock_usart_driver.h"

/* ============================================================================================== */

void helper_uart_create(uart_t* uart)
{
    uart->initialize      = usart1_initialize;
    uart->transmit        = usart1_transmit;
    uart->receive         = usart1_receive;
    uart->set_rx_callback = usart1_set_rx_callback;
    uart->clear_buffers   = usart1_clear_buffers;
}

void mock_uart_callback(const uint8_t* buffer, size_t length)
{
    // Mock implementation of the UART callback function
}

/* ============================================================================================== */

void test_uart_creation(void)
{
    uart_t uart;
    helper_uart_create(&uart);

    TEST_ASSERT_NOT_NULL(uart.initialize);
    TEST_ASSERT_NOT_NULL(uart.transmit);
    TEST_ASSERT_NOT_NULL(uart.receive);
    TEST_ASSERT_NOT_NULL(uart.set_rx_callback);
    TEST_ASSERT_NOT_NULL(uart.clear_buffers);
    TEST_ASSERT_EQUAL_PTR(uart.initialize, usart1_initialize);
    TEST_ASSERT_EQUAL_PTR(uart.transmit, usart1_transmit);
    TEST_ASSERT_EQUAL_PTR(uart.receive, usart1_receive);
    TEST_ASSERT_EQUAL_PTR(uart.set_rx_callback, usart1_set_rx_callback);
    TEST_ASSERT_EQUAL_PTR(uart.clear_buffers, usart1_clear_buffers);
}

/* ============================================================================================== */

void test_uart_callback(void)
{
    uart_t uart;
    helper_uart_create(&uart);

    usart1_set_rx_callback_ExpectAndReturn(mock_uart_callback, 0);
    TEST_ASSERT_EQUAL(0, uart.set_rx_callback(mock_uart_callback));
}

/* ============================================================================================== */

void test_uart_methods(void)
{
    uart_t uart;
    helper_uart_create(&uart);

    usart1_initialize_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL(0, uart.initialize());

    uint8_t tx_buffer[2] = {0x12, 0x34};
    usart1_transmit_ExpectAndReturn(tx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, uart.transmit(tx_buffer, 2));

    uint8_t rx_buffer[2] = {0};
    usart1_receive_ExpectAndReturn(rx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, uart.receive(rx_buffer, 2));

    usart1_set_rx_callback_ExpectAndReturn(mock_uart_callback, 0);
    TEST_ASSERT_EQUAL(0, uart.set_rx_callback(mock_uart_callback));

    usart1_clear_buffers_Expect();
    uart.clear_buffers();
}

/* ============================================================================================== */
