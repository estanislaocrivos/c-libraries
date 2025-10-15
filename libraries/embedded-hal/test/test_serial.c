#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/serial.h"

/* Mocked interfaces */
#include "mock_usart_driver.h"

/* ============================================================================================== */

void helper_uart_create(struct serial_ops* ops, struct serial_port* uart)
{
    ops->initialize          = usart1_initialize;
    ops->transmit            = usart1_transmit;
    ops->receive             = usart1_receive;
    ops->enable_rx_interrupt = usart1_enable_rx_interrupt;
    ops->set_rx_callback     = usart1_set_rx_callback;
    ops->flush_tx            = usart1_flush_tx;
    ops->flush_rx            = usart1_flush_rx;
    uart->ops                = ops;
    uart->port_id            = 0;
    uart->baud_rate          = 115200;
    uart->_callback_context  = NULL;
}

void mock_uart_callback(void* context, uint8_t byte)
{
    // Mock implementation of the UART callback function
    (void)context;  // Unused
    (void)byte;     // Unused
}

/* ============================================================================================== */

void test_uart_creation(void)
{
    struct serial_port uart = {0};
    struct serial_ops  ops  = {0};
    helper_uart_create(&ops, &uart);

    TEST_ASSERT_NOT_NULL(uart.ops->initialize);
    TEST_ASSERT_NOT_NULL(uart.ops->transmit);
    TEST_ASSERT_NOT_NULL(uart.ops->receive);
    TEST_ASSERT_NOT_NULL(uart.ops->set_rx_callback);
    TEST_ASSERT_NOT_NULL(uart.ops->flush_tx);
    TEST_ASSERT_NOT_NULL(uart.ops->flush_rx);
    TEST_ASSERT_EQUAL_PTR(uart.ops->initialize, usart1_initialize);
    TEST_ASSERT_EQUAL_PTR(uart.ops->transmit, usart1_transmit);
    TEST_ASSERT_EQUAL_PTR(uart.ops->receive, usart1_receive);
    TEST_ASSERT_EQUAL_PTR(uart.ops->set_rx_callback, usart1_set_rx_callback);
    TEST_ASSERT_EQUAL_PTR(uart.ops->flush_tx, usart1_flush_tx);
    TEST_ASSERT_EQUAL_PTR(uart.ops->flush_rx, usart1_flush_rx);
}

/* ============================================================================================== */

void test_uart_callback(void)
{
    struct serial_port uart = {0};
    struct serial_ops  ops  = {0};
    helper_uart_create(&ops, &uart);

    usart1_set_rx_callback_ExpectAndReturn(&uart, mock_uart_callback, NULL, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->set_rx_callback(&uart, mock_uart_callback, NULL));
}

/* ============================================================================================== */

void test_uart_methods(void)
{
    struct serial_port uart = {0};
    struct serial_ops  ops  = {0};
    helper_uart_create(&ops, &uart);

    usart1_initialize_ExpectAndReturn(&uart, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->initialize(&uart));

    uint8_t tx_buffer[2] = {0x12, 0x34};
    usart1_transmit_ExpectAndReturn(&uart, tx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->transmit(&uart, tx_buffer, 2));

    uint8_t rx_buffer;
    usart1_receive_ExpectAndReturn(&uart, &rx_buffer, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->receive(&uart, &rx_buffer));

    usart1_set_rx_callback_ExpectAndReturn(&uart, mock_uart_callback, NULL, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->set_rx_callback(&uart, mock_uart_callback, NULL));

    usart1_flush_tx_ExpectAndReturn(&uart, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->flush_tx(&uart));

    usart1_flush_rx_ExpectAndReturn(&uart, 0);
    TEST_ASSERT_EQUAL(0, uart.ops->flush_rx(&uart));
}

/* ============================================================================================== */
